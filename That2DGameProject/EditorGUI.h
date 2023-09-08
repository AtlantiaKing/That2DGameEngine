#pragma once

#include "Singleton.h"

#include <functional>
#include <string>
#include <memory>

#include "glm/vec2.hpp"

struct SDL_Renderer;

namespace that
{
	class Font;

	struct Button
	{
		int x{}, y{}, width{}, height{};
		std::function<void()> onClick{};

		void TryClick(const glm::ivec2& point) const
		{
			if (point.x < x || point.x > x + width) return;
			if (point.y < y || point.y > y + height) return;
			onClick();
		}
	};

	class EditorGUI final : public Singleton<EditorGUI>
	{
	public:
		void RenderText(SDL_Renderer* pRenderer, const std::string& text, int& curY) const;
		void RenderButton(SDL_Renderer* pRenderer, const std::string& text, int& curY, Button& button) const;

	private:
		EditorGUI();
		virtual ~EditorGUI() = default;

		std::shared_ptr<that::Font> m_pFont{};

		friend Singleton<EditorGUI>;
	};
}

