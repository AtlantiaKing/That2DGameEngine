#pragma once

#include "WindowComponent.h"

#include "EditorGUI.h"

#include <vector>

namespace that
{
	class EditorToolsWindow final : public WindowComponent
	{
	public:
		EditorToolsWindow() = default;
		virtual ~EditorToolsWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) override;
		virtual void OnMouseButton(int mouseButton, bool released, const glm::ivec2& point) override;

	private:
		std::vector<Button> m_Buttons{};
	};
}

