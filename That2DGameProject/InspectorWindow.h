#pragma once

#include "WindowComponent.h"

#include <string>
#include <memory>
#include <functional>

#include "EditorGUI.h"

namespace that
{
	class GameObject;

	class InspectorWindow final : public WindowComponent
	{
	public:
		InspectorWindow();
		virtual ~InspectorWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) override;
		virtual void OnClick(const glm::ivec2& point) override;

		void SetGameObject(GameObject* pGameObject);

	private:
		GameObject* m_pWatchingObject{};

		bool m_IsShowingComponents{ false };
		Button m_ComponentButton{};
		std::vector<Button> m_AddComponents{};
	};
}
