#pragma once

#include <Singleton.h>

#include <memory>
#include <string>
#include <vector>

#include "StandaloneWindow.h"

namespace that
{
	class GameObject;

	class Editor final : public Singleton<Editor>
	{
	public:
		void Init(const std::string& dataPath);
		void UpdateVisuals();

		GameObject* GetSelectedObject() const { return m_pSelectedObject; }
		void SetSelectedObject(GameObject* pGameObject) { m_pSelectedObject = pGameObject; }
	private:
		Editor() = default;
		~Editor();

		std::vector<StandaloneWindow> m_Windows{};

		GameObject* m_pSelectedObject{};

		void QuitWindow(Uint32 windowId);
		void MouseButtonWindow(Uint32 windowId, int mouseButton, bool released, const glm::ivec2& point) const;
		void MouseMovementWindow(Uint32 windowId, const glm::ivec2& displacement) const;

		glm::ivec2 m_PrevMousePos{};

		friend Singleton<Editor>;
	};
}

