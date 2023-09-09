#pragma once

#include "WindowComponent.h"

#include <vector>

#include "EditorGUI.h"

#include "glm/vec2.hpp"

namespace that
{
	class SceneWindow final : public WindowComponent
	{
	public:
		SceneWindow() = default;
		virtual ~SceneWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) override;
		virtual void OnMouseButton(int mouseButton, bool released, const glm::ivec2& point) override;
		virtual void OnMouseMovement(const glm::ivec2& displacement) override;

	private:
		bool m_Initialised{};

		bool m_MoveX{};
		bool m_MoveY{};

		std::vector<Button> m_Buttons{};
	};
}
