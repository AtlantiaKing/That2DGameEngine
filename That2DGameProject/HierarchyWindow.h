#pragma once

#include "WindowComponent.h"

#include <string>
#include <memory>

namespace that
{
	class Font;

	class HierarchyWindow final : public WindowComponent
	{
	public:
		HierarchyWindow();
		virtual ~HierarchyWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) const override;

	private:
		void RenderText(SDL_Renderer* pRenderer, const std::string& text, int& curY) const;

		std::shared_ptr<that::Font> m_pFont{};
	};
}
