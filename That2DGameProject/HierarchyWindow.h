#pragma once

#include "WindowComponent.h"

#include <string>
#include <memory>
#include <functional>

#include "EditorGUI.h"

namespace that
{
	class InspectorWindow;
	class GameObject;

	class HierarchyWindow final : public WindowComponent
	{
	public:
		HierarchyWindow() = default;
		virtual ~HierarchyWindow() = default;

		void SetInspector(InspectorWindow* pInspector);

		virtual void Render(SDL_Renderer* pWindow) override;
		virtual void OnMouseButton(int mouseButton, bool released, const glm::ivec2& point) override;

	private:
		void RenderObject(SDL_Renderer* pWindow, GameObject* pGameObject, int& curY, const std::string& curSpacing);

		std::vector<Button> m_Buttons{};
		std::vector<Button> m_ControlButtons{};

		InspectorWindow* m_pInspector{};

		bool m_ShowControlMenu{};
		bool m_ShowNewMenu{};
		glm::ivec2 m_MenuPosition{};
		GameObject* m_pControllingGameObject{};
	};
}
