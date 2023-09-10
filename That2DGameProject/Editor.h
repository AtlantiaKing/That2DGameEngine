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
		
		template<typename T>
		bool HasWindowWithComponent() const
		{
			const auto it{ std::find_if(begin(m_Windows), end(m_Windows),
										[](const auto& window)
										{
											return window.GetComponent<T>() != nullptr;
										}
						) };

			return it != end(m_Windows);
		}
		
		StandaloneWindow* CreateWindow(const std::string& title, const glm::ivec2& size);

	private:
		Editor() = default;
		~Editor();

		void Shutdown();

		Uint32 m_ToolsWindow{};
		std::vector<StandaloneWindow> m_Windows{};

		GameObject* m_pSelectedObject{};

		void QuitWindow(Uint32 windowId);
		void MouseButtonWindow(Uint32 windowId, int mouseButton, bool released, const glm::ivec2& point) const;
		void MouseMovementWindow(Uint32 windowId, const glm::ivec2& displacement) const;

		glm::ivec2 m_PrevMousePos{};

		friend Singleton<Editor>;
	};
}

