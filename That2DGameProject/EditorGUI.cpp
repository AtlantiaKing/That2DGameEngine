#include "EditorGUI.h"

#include "ResourceManager.h"
#include "Font.h"
#include "Texture2D.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <iostream>

that::EditorGUI::EditorGUI()
{
	m_pFont = ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8);
}

void that::EditorGUI::RenderText(SDL_Renderer* pRenderer, const std::string& text, int& curY) const
{
	// Create a SDL surface using the current font, text and color
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (!pSurface) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	// Create a texture from the created surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
	if (!pTexture) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	// Release the surface
	SDL_FreeSurface(pSurface);

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = curY;
	curY += dst.h;

	// Render the text to the window
	SDL_RenderCopy(pRenderer, pTexture, nullptr, &dst);

	// Release the texture
	SDL_DestroyTexture(pTexture);
}

void that::EditorGUI::RenderButton(SDL_Renderer* pRenderer, const std::string& text, int& curY, Button& button) const
{
	RenderButton(pRenderer, text, curY, 0, button);
}

void that::EditorGUI::RenderButton(SDL_Renderer* pRenderer, const std::string& text, int& curY, int x, Button& button) const
{
	// Create a SDL surface using the current font, text and color
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (!pSurface) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	// Create a texture from the created surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
	if (!pTexture) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	// Release the surface
	SDL_FreeSurface(pSurface);

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = curY;
	dst.x = x;
	curY += dst.h;

	// Store the destination rect info in the button
	button.x = dst.x;
	button.y = dst.y;
	button.width = dst.w;
	button.height = dst.h;

	// Render the text to the window
	SDL_RenderCopy(pRenderer, pTexture, nullptr, &dst);

	// Release the texture
	SDL_DestroyTexture(pTexture);
}

void that::EditorGUI::RenderImage(SDL_Renderer* pRenderer, const std::string& path, int x, int y, float pivotX, float pivotY) const
{
	Button temp{};
	RenderImage(pRenderer, path, x, y, pivotX, pivotY, temp);
}

void that::EditorGUI::RenderImage(SDL_Renderer* pRenderer, const std::string& path, int x, int y, float pivotX, float pivotY, Button& button) const
{
	// Create a texture from the created surface
	auto pTexture = that::ResourceManager::GetInstance().LoadTexture(path, pRenderer);
	if (!pTexture) throw std::runtime_error(std::string("Create texture from file failed: ") + SDL_GetError());

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture->GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.x = x - static_cast<int>(pivotX * dst.w);
	dst.y = y - static_cast<int>(pivotY * dst.h);

	// Store the destination rect info in the button
	button.x = dst.x;
	button.y = dst.y;
	button.width = dst.w;
	button.height = dst.h;

	// Render the text to the window
	SDL_RenderCopy(pRenderer, pTexture->GetSDLTexture(), nullptr, &dst);
}
