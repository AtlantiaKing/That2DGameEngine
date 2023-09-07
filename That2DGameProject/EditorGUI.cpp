#include "EditorGUI.h"

#include "ResourceManager.h"
#include "Font.h"

#include <SDL_ttf.h>

#include <stdexcept>

that::EditorGUI::EditorGUI()
{
	m_pFont = ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8);
}

void that::EditorGUI::RenderText(SDL_Renderer* pRenderer, const std::string& text, int& curY) const
{
	// Create a texture using the current font, text and color
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	const auto texture = SDL_CreateTextureFromSurface(pRenderer, surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	SDL_Rect dst{};
	SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = curY;
	curY += dst.h;

	SDL_RenderCopy(pRenderer, texture, nullptr, &dst);

	SDL_DestroyTexture(texture);
}

void that::EditorGUI::RenderButton(SDL_Renderer* pRenderer, const std::string& text, int& curY, Button& button)
{
	// Create a texture using the current font, text and color
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	const auto texture = SDL_CreateTextureFromSurface(pRenderer, surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	SDL_Rect dst{};
	SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = curY;
	curY += dst.h;

	button.x = dst.x;
	button.y = dst.y;
	button.width = dst.w;
	button.height = dst.h;

	SDL_RenderCopy(pRenderer, texture, nullptr, &dst);

	SDL_DestroyTexture(texture);
}
