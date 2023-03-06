#include "TextComponent.h"
#include "Font.h"
#include <stdexcept>
#include "Renderer.h"
#include "TextureRenderer.h"

void that::TextComponent::SetFont(std::shared_ptr<Font> pFont)
{
	// The font didn't change, so do nothing
	if (m_pFont.get() == pFont.get()) return;

	m_pFont = pFont;
	m_HasChanged = true;
}

void that::TextComponent::SetText(const std::string& text)
{
	// The text didn't change, so do nothing
	if (m_Text == text) return;

	m_Text = text;
	m_HasChanged = true;
}

void that::TextComponent::SetColor(const SDL_Color& color)
{
	// The color didn't change, so do nothing
	if (m_Color.r == color.r && m_Color.g == color.g && m_Color.b == color.b && m_Color.a == color.a) return;

	m_Color = color;
	m_HasChanged = true;
}

void that::TextComponent::SetColor(int r, int g, int b)
{
	// The color didn't change, so do nothing
	if (m_Color.r == r && m_Color.g == g && m_Color.b == b && m_Color.a == UINT8_MAX) return;

	m_Color = SDL_Color
	{ 
		static_cast<unsigned char>(r),
		static_cast<unsigned char>(g),
		static_cast<unsigned char>(b) 
	};
	m_HasChanged = true;
}

void that::TextComponent::SetColor(int r, int g, int b, int a)
{
	// The color didn't change, so do nothing
	if (m_Color.r == r && m_Color.g == g && m_Color.b == b && m_Color.a == a) return;

	m_Color = SDL_Color
	{ 
		static_cast<unsigned char>(r),
		static_cast<unsigned char>(g),
		static_cast<unsigned char>(b),
		static_cast<unsigned char>(a) 
	};
	m_HasChanged = true;
}

void that::TextComponent::Update()
{
	// If the texture should not be reloaded, do nothing
	if (!m_HasChanged) return;

	// Try to reload the texture and get the result of the function
	bool reloadSucces{ ReloadTexture() };

	// If the reload was not a succes, try again next frame
	// If the reload succeeded, set hasChanged to false
	m_HasChanged = !reloadSucces;
}

bool that::TextComponent::ReloadTexture()
{
	// If no font has been assigned, do nothing
	// TODO: Log a warning that no font is assigned to this TextComponent
	if (!m_pFont) return true;

	// If no texture renderer is assigned, try getting a texture renderer from the parent
	//	If no texture renderer is found on the parent, do nothing
	// TODO: Log a warning that no TextureRenderer is assigned to this TextComponent
	if (m_pTextureRenderer.expired())
	{
		m_pTextureRenderer = GetComponent<TextureRenderer>();

		if (m_pTextureRenderer.expired()) return false;
	};

	// Create a texture using the current font, text and color
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	const auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	// Set the new texture to the texture renderer
	m_pTextureRenderer.lock()->SetTexture(std::make_shared<Texture2D>(texture));

	// Return ReloadTexture succeeded
	return true;
}
