#pragma once

#include "Singleton.h"

#include <functional>
#include <string>
#include <memory>
#include <map>

#include "glm/vec2.hpp"

struct SDL_Renderer;

namespace that
{
	class Font;

	struct Button
	{
		int x{}, y{}, width{}, height{};
		std::function<void()> onClick{};
		std::function<void()> onAltClick{};

		bool TryClick(const glm::ivec2& point) const
		{
			if (!DidClick(point)) return false;
			onClick();
			return true;
		}
		bool TryAltClick(const glm::ivec2& point) const
		{
			if (!DidClick(point)) return false;
			onAltClick();
			return true;
		}
		bool DidClick(const glm::ivec2& point) const
		{
			if (point.x < x || point.x > x + width) return false;
			if (point.y < y || point.y > y + height) return false;
			return true;
		}
	};

	class EditorGUI final : public Singleton<EditorGUI>
	{
	public:
		void Begin(SDL_Renderer* pRenderer);

		void Space(int ySpacing);
		void Move(int y);

		void RenderText(const std::string& text);
		void RenderText(const std::string& text, int xOffset);

		void RenderInputField(const std::string& curText);

		bool RenderButton(const std::string& text);
		bool RenderButton(const std::string& text, int x);
		int RenderMultiButton(const std::string& text);
		int RenderMultiButton(const std::string& text, int x);
		int RenderMultiButtonFullWidth(const std::string& text);

		void RenderImage(const std::string& path, int x, int y, float pivotX, float pivotY) const;
		void RenderImage(const std::string& path, int x, int y, float pivotX, float pivotY, Button& button) const;

		void SetMargin(int x, int y);
		void SetCursor(const glm::ivec2& cursorPosition);
		void SetIsClicking(uint32_t clickedWindow);
		void SetIsAltClicking(uint32_t clickedWindow);

		const glm::ivec2& GetCurserPosition() { return m_CurserPos; }
		bool HasClickedWindow() const;
		bool HasAltClickedWindow() const;
	private:
		struct Color
		{
			uint8_t r{}, g{}, b{}, a{};
		};

		EditorGUI();
		virtual ~EditorGUI() = default;

		void ResetButtonPresses();

		std::shared_ptr<that::Font> m_pFont{};

		SDL_Renderer* m_pCurRenderer{};
		uint32_t m_CurWindow{};
		int m_YValue{};
		bool m_ClickedButton{};
		bool m_AltClickedButton{};

		glm::ivec2 m_Margin{ 2 };

		Color m_TextColor{ 255, 255, 255, 255 };
		Color m_AltColor{ 50, 50, 50, 255 };
		Color m_BorderColor{ 0, 0, 0, 255 };

		glm::ivec2 m_CurserPos{};
		uint32_t m_IsClicking{ UINT_MAX };
		uint32_t m_IsAltClicking{ UINT_MAX };

		friend Singleton<EditorGUI>;

		std::map<uint32_t, std::map<int, int>> m_BackgroundButtonPresses{};
		std::map<uint32_t, std::map<int, int>> m_ButtonPresses{};
	};
}

