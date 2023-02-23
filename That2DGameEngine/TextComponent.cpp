#include "TextComponent.h"
#include "Font.h"
#include <stdexcept>
#include "Renderer.h"
#include "TextureRenderer.h"

void that::TextComponent::SetFont(std::shared_ptr<Font> pFont)
{
	m_pFont = pFont;
	ReloadTexture();
}

void that::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	ReloadTexture();
}

void that::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	ReloadTexture();
}

void that::TextComponent::SetColor(int r, int g, int b)
{
	m_Color = SDL_Color
	{ 
		static_cast<unsigned char>(r),
		static_cast<unsigned char>(g),
		static_cast<unsigned char>(b) 
	};
	ReloadTexture();
}

void that::TextComponent::SetColor(int r, int g, int b, int a)
{
	m_Color = SDL_Color
	{ 
		static_cast<unsigned char>(r),
		static_cast<unsigned char>(g),
		static_cast<unsigned char>(b),
		static_cast<unsigned char>(a) 
	};
	ReloadTexture();
}

void that::TextComponent::ReloadTexture()
{
	// If no font has been assigned, do nothing
	// TODO: Log a warning that no font is assigned to this TextComponent
	if (!m_pFont) return;

	// If no texture renderer is assigned, try getting a texture renderer from the parent
	//	If no texture renderer is found on the parent, do nothing
	// TODO: Log a warning that no TextureRenderer is assigned to this TextComponent
	if (m_pTextureRenderer.expired())
	{
		m_pTextureRenderer = GetComponent<TextureRenderer>();

		if (m_pTextureRenderer.expired()) return;
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
}
