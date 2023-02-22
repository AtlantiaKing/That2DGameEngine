#pragma once

#include "Component.h"
#include <SDL_ttf.h>
#include <string>

namespace that
{
	class Font;

	class TextComponent final : public Component
	{
	public:
		TextComponent() = default;
		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		const std::string& GetText() const { return m_Text; };
		const SDL_Color& GetColor() const { return m_Color; };

		void SetFont(std::shared_ptr<Font> pFont);
		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);
		void SetColor(int r, int g, int b);
		void SetColor(int r, int g, int b, int a);
	private:
		void ReloadTexture();

		std::string m_Text{ " " };
		SDL_Color m_Color{ 255,255,255 };
		std::shared_ptr<Font> m_pFont{};

		std::weak_ptr<TextureRenderer> m_pTextureRenderer{};
	};
}

