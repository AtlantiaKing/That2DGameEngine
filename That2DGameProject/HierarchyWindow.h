#pragma once

#include "WindowComponent.h"

#include <string>
#include <memory>
#include <functional>

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

namespace that
{
	class GameObject;
	class Font;

	class HierarchyWindow final : public WindowComponent
	{
	public:
		HierarchyWindow();
		virtual ~HierarchyWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) override;
		virtual void OnClick(const glm::ivec2& point) override;

		void SetGameObject(GameObject* pGameObject);

	private:
		void RenderText(SDL_Renderer* pRenderer, const std::string& text, int& curY) const;
		void RenderButton(SDL_Renderer* pRenderer, const std::string& text, int& curY, Button& button);

		std::shared_ptr<that::Font> m_pFont{};
		GameObject* m_pWatchingObject{};

		bool m_IsShowingComponents{ false };
		Button m_ComponentButton{};
		std::vector<Button> m_AddComponents{};
	};
}
