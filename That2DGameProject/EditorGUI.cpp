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
	m_pFont = ResourceManager::GetInstance().LoadFont("Editor/font.ttf", 14);
}

void that::EditorGUI::RenderText(const std::string& text)
{
	RenderText(text, 0);
}

void that::EditorGUI::RenderText(const std::string& text, int xOffset)
{
	// Create a SDL surface using the current font, text and color
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a });
	if (!pSurface) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	// Create a texture from the created surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pCurRenderer, pSurface);
	if (!pTexture) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	// Release the surface
	SDL_FreeSurface(pSurface);

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture, nullptr, nullptr, &dst.w, &dst.h);
	dst.x = m_Margin.x + xOffset;
	dst.y = m_YValue;
	Space(dst.h);

	// Render the text to the window
	SDL_RenderCopy(m_pCurRenderer, pTexture, nullptr, &dst);

	// Release the texture
	SDL_DestroyTexture(pTexture);

	Space(1);
}

void that::EditorGUI::RenderInputField(const std::string& curText)
{
	const unsigned int inputFieldSize{ m_pFont->GetSize() + 6 };

	SDL_Rect backgroundRect{ m_Margin.x, m_YValue };
	SDL_GetWindowSize(SDL_RenderGetWindow(m_pCurRenderer), &backgroundRect.w, nullptr);
	backgroundRect.w -= m_Margin.x * 2;
	backgroundRect.h = inputFieldSize;

	SDL_SetRenderDrawColor(m_pCurRenderer, m_AltColor.r, m_AltColor.g, m_AltColor.b, m_AltColor.a);
	SDL_RenderFillRect(m_pCurRenderer, &backgroundRect);
	SDL_SetRenderDrawColor(m_pCurRenderer, m_BorderColor.r, m_BorderColor.g, m_BorderColor.b, m_BorderColor.a);
	SDL_RenderDrawRect(m_pCurRenderer, &backgroundRect);

	RenderText(curText, 3);

	Space(3);
	Space(1);
}

bool that::EditorGUI::RenderButton(const std::string& text)
{
	return RenderButton(text, m_Margin.x);
}

bool that::EditorGUI::RenderButton(const std::string& text, int x)
{
	// Create a SDL surface using the current font, text and color
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (!pSurface) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	// Create a texture from the created surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pCurRenderer, pSurface);
	if (!pTexture) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	// Release the surface
	SDL_FreeSurface(pSurface);

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = m_YValue;
	dst.x = x;
	Space(dst.h);

	// Render the text to the window
	SDL_RenderCopy(m_pCurRenderer, pTexture, nullptr, &dst);

	// Release the texture
	SDL_DestroyTexture(pTexture);

	Space(1);

	const int buttonHash{ dst.x + dst.y };

	const bool prevClick{ m_ButtonPresses[m_CurWindow].contains(buttonHash) && m_ButtonPresses[m_CurWindow][buttonHash] == 0};

	const bool isClicked{ m_IsClicking == m_CurWindow &&
					m_CurserPos.x > dst.x && m_CurserPos.x < dst.x + dst.w &&
					m_CurserPos.y > dst.y && m_CurserPos.y < dst.y + dst.h };
	if (isClicked)
	{
		ResetButtonPresses();
	}
	m_BackgroundButtonPresses[m_CurWindow][buttonHash] = isClicked ? 0 : -1;

	return prevClick;
}

void that::EditorGUI::ResetButtonPresses()
{
	for (auto& button : m_BackgroundButtonPresses[m_CurWindow])
	{
		button.second = -1;
	}
}

int that::EditorGUI::RenderMultiButton(const std::string& text)
{
	return RenderMultiButton(text, m_Margin.x);
}

int that::EditorGUI::RenderMultiButton(const std::string& text, int x)
{
	// Create a SDL surface using the current font, text and color
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (!pSurface) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	// Create a texture from the created surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pCurRenderer, pSurface);
	if (!pTexture) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	// Release the surface
	SDL_FreeSurface(pSurface);

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = m_YValue;
	dst.x = x;
	Space(dst.h);

	// Render the text to the window
	SDL_RenderCopy(m_pCurRenderer, pTexture, nullptr, &dst);

	// Release the texture
	SDL_DestroyTexture(pTexture);

	Space(1);

	const int buttonHash{ dst.x + dst.y };

	const int prevClick{ m_ButtonPresses[m_CurWindow].contains(buttonHash) ? m_ButtonPresses[m_CurWindow][buttonHash] : -1 };

	const bool isClicked{ (m_IsClicking == m_CurWindow || m_IsAltClicking) &&
					m_CurserPos.x > dst.x && m_CurserPos.x < dst.x + dst.w &&
					m_CurserPos.y > dst.y && m_CurserPos.y < dst.y + dst.h };

	if (isClicked)
	{
		ResetButtonPresses();
	}
	m_BackgroundButtonPresses[m_CurWindow][buttonHash] = isClicked ? (m_IsClicking == m_CurWindow ? 0 : (m_IsAltClicking == m_CurWindow ? 1 : -1)) : -1;

	return prevClick;
}

int that::EditorGUI::RenderMultiButtonFullWidth(const std::string& text)
{
	const unsigned int buttonSize{ m_pFont->GetSize() + 6 };

	SDL_Rect backgroundRect{ 0, m_YValue };
	SDL_GetWindowSize(SDL_RenderGetWindow(m_pCurRenderer), &backgroundRect.w, nullptr);
	backgroundRect.h = buttonSize;

	SDL_SetRenderDrawColor(m_pCurRenderer, m_AltColor.r, m_AltColor.g, m_AltColor.b, m_AltColor.a);
	SDL_RenderFillRect(m_pCurRenderer, &backgroundRect);
	SDL_SetRenderDrawColor(m_pCurRenderer, m_BorderColor.r, m_BorderColor.g, m_BorderColor.b, m_BorderColor.a);
	SDL_RenderDrawRect(m_pCurRenderer, &backgroundRect);

	// Create a SDL surface using the current font, text and color
	SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), SDL_Color{ 255,255,255,255 });
	if (!pSurface) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	// Create a texture from the created surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pCurRenderer, pSurface);
	if (!pTexture) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

	// Release the surface
	SDL_FreeSurface(pSurface);

	// Calculate the bounds of the destination rect
	SDL_Rect dst{};
	SDL_QueryTexture(pTexture, nullptr, nullptr, &dst.w, &dst.h);
	dst.y = m_YValue;
	dst.x = m_Margin.x;
	Space(dst.h);

	// Render the text to the window
	SDL_RenderCopy(m_pCurRenderer, pTexture, nullptr, &dst);

	// Release the texture
	SDL_DestroyTexture(pTexture);

	Space(1);

	const int buttonHash{ dst.x + dst.y };

	const int prevClick{ m_ButtonPresses[m_CurWindow].contains(buttonHash) ? m_ButtonPresses[m_CurWindow][buttonHash] : -1 };

	const bool isClicked{ (m_IsClicking == m_CurWindow || m_IsAltClicking == m_CurWindow) &&
					m_CurserPos.x > dst.x && m_CurserPos.x < dst.x + dst.w &&
					m_CurserPos.y > dst.y && m_CurserPos.y < dst.y + dst.h };

	if (isClicked)
	{
		ResetButtonPresses();
	}
	m_BackgroundButtonPresses[m_CurWindow][buttonHash] = isClicked ? (m_IsClicking == m_CurWindow ? 0 : (m_IsAltClicking == m_CurWindow ? 1 : -1)) : -1;

	return prevClick;
}

void that::EditorGUI::RenderImage(const std::string& path, int x, int y, float pivotX, float pivotY) const
{
	Button temp{};
	RenderImage(path, x, y, pivotX, pivotY, temp);
}

void that::EditorGUI::RenderImage(const std::string& path, int x, int y, float pivotX, float pivotY, Button& button) const
{
	// Create a texture from the created surface
	auto pTexture = that::ResourceManager::GetInstance().LoadTexture(path, m_pCurRenderer);
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
	SDL_RenderCopy(m_pCurRenderer, pTexture->GetSDLTexture(), nullptr, &dst);
}

void that::EditorGUI::SetMargin(int x, int y)
{
	m_Margin = { x, y };
}

void that::EditorGUI::SetCursor(const glm::ivec2& cursorPosition)
{
	m_CurserPos = cursorPosition;
}

void that::EditorGUI::SetIsClicking(uint32_t clickedWindow)
{
	m_IsClicking = clickedWindow;
}

void that::EditorGUI::SetIsAltClicking(uint32_t clickedWindow)
{
	m_IsAltClicking = clickedWindow;
}

bool that::EditorGUI::HasClickedWindow() const
{
	if (m_IsClicking != m_CurWindow) return false;

	bool buttonPress{};
	for (const auto& button : m_BackgroundButtonPresses.at(m_CurWindow))
	{
		if (button.second > -1)
		{
			buttonPress = true;
			break;
		}
	}

	return !buttonPress;
}

bool that::EditorGUI::HasAltClickedWindow() const
{
	if (m_IsAltClicking != m_CurWindow) return false;

	bool buttonPress{};
	for (const auto& button : m_BackgroundButtonPresses.at(m_CurWindow))
	{
		if (button.second > -1)
		{
			buttonPress = true;
			break;
		}
	}

	return !buttonPress;
}

void that::EditorGUI::Begin(SDL_Renderer* pRenderer)
{
	m_pCurRenderer = pRenderer;
	m_CurWindow = SDL_GetWindowID(SDL_RenderGetWindow(pRenderer));

	// Make sure both buffers have a dictionary for the current window
	if (!m_BackgroundButtonPresses.contains(m_CurWindow))
	{
		std::map<int, int> newDictionary{};
		m_BackgroundButtonPresses.insert(std::make_pair(m_CurWindow, newDictionary));
		m_ButtonPresses.insert(std::make_pair(m_CurWindow, newDictionary));
	}

	// Swap the buffers of button presses
	m_ButtonPresses[m_CurWindow] = m_BackgroundButtonPresses[m_CurWindow];
	ResetButtonPresses();

	m_YValue = m_Margin.y;

	m_ClickedButton = false;
	m_AltClickedButton = false;
}

void that::EditorGUI::Space(int ySpacing)
{
	m_YValue += ySpacing;
}

void that::EditorGUI::Move(int y)
{
	m_YValue = y;
}
