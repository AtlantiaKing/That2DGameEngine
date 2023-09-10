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

		virtual void Render(SDL_Renderer* pWindow) override;

	private:
		void RenderObject(GameObject* pGameObject, const std::string& curSpacing);

		InspectorWindow* m_pInspector{};

		bool m_ShowControlMenu{};
		bool m_ShowNewMenu{};
		glm::ivec2 m_MenuPosition{};
		GameObject* m_pControllingGameObject{};
	};
}
