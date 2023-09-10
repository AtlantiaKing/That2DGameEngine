#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Surface2D.h"

void that::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<that::Texture2D> that::ResourceManager::LoadTexture(const std::string& file) const
{
	return LoadTexture(file, Renderer::GetInstance().GetSDLRenderer());
}

std::shared_ptr<that::Texture2D> that::ResourceManager::LoadTexture(const std::string& file, SDL_Renderer* pRenderer) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(pRenderer, fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<that::Font> that::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	FontData fontPair{ std::make_pair(file,size) };

	if (const auto pFontIt{ std::find_if(begin(m_pFonts), end(m_pFonts), [&](const auto& fontData) { return fontData.first == fontPair; })}; pFontIt != end(m_pFonts))
	{
		return pFontIt->second;
	}

	auto pFont{ std::make_shared<Font>(m_dataPath + file, size) };
	m_pFonts.push_back(std::make_pair(fontPair, pFont));

	return pFont;
}

std::shared_ptr<that::Surface2D> that::ResourceManager::LoadSurface(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto pSurface = IMG_Load(fullPath.c_str());
	if (pSurface == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load surface: ") + SDL_GetError());
	}
	return std::make_shared<Surface2D>(pSurface);
}
