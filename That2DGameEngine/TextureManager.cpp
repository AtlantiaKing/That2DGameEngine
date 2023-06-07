#include "TextureManager.h"

#include "ResourceManager.h"

std::shared_ptr<that::Texture2D> that::TextureManager::LoadTexture(const std::string& file)
{
	if (const auto pTextureIt{ m_pTextures.find(file) }; pTextureIt != end(m_pTextures))
	{
		return pTextureIt->second;
	}

	auto pTexture{ that::ResourceManager::GetInstance().LoadTexture(file) };
	m_pTextures[file] = pTexture;

	return pTexture;
}
