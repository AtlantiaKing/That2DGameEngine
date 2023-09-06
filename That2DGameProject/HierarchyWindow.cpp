#include "HierarchyWindow.h"
#include <iostream>

#include "Font.h"
#include "Texture2D.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "ResourceManager.h"

#include "Reflection.h"

that::HierarchyWindow::HierarchyWindow()
{
	m_pFont = that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 10);
}

void that::HierarchyWindow::Render(SDL_Renderer* pRenderer) const
{
	const auto& allTypes{ that::reflection::Reflection::GetTypes() };

	int curStartY{};

	for (const auto& type : allTypes)
	{
		RenderText(pRenderer, type.name, curStartY);
		curStartY += 3;

		for (const auto& member : type.variables)
		{
			RenderText(pRenderer, "   " + member.name, curStartY);
			curStartY += 3;
		}
		curStartY += 10;
	}
}

void that::HierarchyWindow::RenderText(SDL_Renderer* pRenderer, const std::string& text, int& curY) const
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
