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
	if (!m_pFont) return;

	if (m_pTextureRenderer.expired())
	{
		m_pTextureRenderer = GetComponent<TextureRenderer>();

		if (m_pTextureRenderer.expired()) return;
	};

	auto pTextureRenderer{ m_pTextureRenderer.lock() };

	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	pTextureRenderer->SetTexture(std::make_shared<Texture2D>(texture));
}
