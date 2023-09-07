#include "HierarchyWindow.h"
#include <iostream>

#include "Font.h"
#include "Texture2D.h"

#include "GameObject.h"
#include "Component.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "ResourceManager.h"

#include "Reflection.h"

that::HierarchyWindow::HierarchyWindow()
{
	m_pFont = that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8);

	m_ComponentButton.onClick = [&]() { m_IsShowingComponents = true; };
}

void that::HierarchyWindow::Render(SDL_Renderer* pRenderer)
{
	if (m_pWatchingObject == nullptr) return;

	const auto& goTypes{ m_pWatchingObject->GetComponents() };

	int curStartY{};
	RenderText(pRenderer, m_pWatchingObject->GetName(), curStartY);
	curStartY += 8;

	for (const auto& type : goTypes)
	{
		const auto& typeinfo{ reflection::Reflection::GetType(type->GetHash()) };

		RenderText(pRenderer, typeinfo.name, curStartY);
		curStartY += 1;

		for (const auto& member : typeinfo.variables)
		{
			RenderText(pRenderer, "   " + member.name, curStartY);
			curStartY += 1;
		}
		curStartY += 3;
	}
	curStartY += 5;
	RenderButton(pRenderer, "Add Component", curStartY, m_ComponentButton);
	curStartY += 5;

	m_AddComponents.clear();
	if (m_IsShowingComponents)
	{
		const auto& allTypes{ reflection::Reflection::GetTypes() };

		for (const auto& type : allTypes)
		{
			Button typeButton{};
			RenderButton(pRenderer, type.name, curStartY, typeButton);
			typeButton.onClick = [&]() 
			{ 
				type.Clone(m_pWatchingObject); 
				m_IsShowingComponents = false;
			};
			m_AddComponents.emplace_back(typeButton);

			curStartY += 1;
		}
	}
}

void that::HierarchyWindow::OnClick(const glm::ivec2& point)
{
	m_IsShowingComponents = false;

	m_ComponentButton.TryClick(point);
	for (const auto& button : m_AddComponents)
	{
		button.TryClick(point);
	}
}

void that::HierarchyWindow::SetGameObject(GameObject* pGameObject)
{
	m_pWatchingObject = pGameObject;
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

void that::HierarchyWindow::RenderButton(SDL_Renderer* pRenderer, const std::string& text, int& curY, Button& button)
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
