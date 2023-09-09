#include "HierarchyWindow.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "InspectorWindow.h"

#include "Editor.h"

void that::HierarchyWindow::SetInspector(InspectorWindow* pInspector)
{
	m_pInspector = pInspector;
}

void that::HierarchyWindow::Render(SDL_Renderer* pWindow)
{
	// Reset all the buttons saved from previous render
	m_Buttons.clear();

	// Get all the objects in the scene
	const auto& pObjects{ SceneManager::GetInstance().GetCurrentScene()->GetObjects() };

	int curStartY{};
	const std::string defaultSpacing{ "" };

	// Render each gameobject
	for (const auto& pObject : pObjects)
	{
		RenderObject(pWindow, pObject, curStartY, defaultSpacing);

		// Add spacing between the gameobjects
		curStartY += 5;
	}

	m_ControlButtons.clear();
	if (m_ShowControlMenu)
	{
		curStartY = m_MenuPosition.y;

		Button button{};

		EditorGUI::GetInstance().RenderButton(pWindow, "Delete", curStartY, m_MenuPosition.x, button);
		button.onClick = [=]()
		{
			if (m_pControllingGameObject->GetParent() == nullptr)
			{
				m_pControllingGameObject->GetScene()->DestroyInstant(m_pControllingGameObject);
			}
			else
			{
				m_pControllingGameObject->GetParent()->DestroyInstant(m_pControllingGameObject);
			}
			Editor::GetInstance().SetSelectedObject(nullptr);
		};
		m_ControlButtons.emplace_back(button);


		EditorGUI::GetInstance().RenderButton(pWindow, "Create child GameObject", curStartY, m_MenuPosition.x, button);
		button.onClick = [=]()
		{
			GameObject* pNewObject{ m_pControllingGameObject->CreateGameObject("Child GameObject") };
			Editor::GetInstance().SetSelectedObject(pNewObject);
		};
		m_ControlButtons.emplace_back(button);
	}
	else if (m_ShowNewMenu)
	{
		curStartY = m_MenuPosition.y;

		Button createButton{};
		EditorGUI::GetInstance().RenderButton(pWindow, "Create new GameObject", curStartY, m_MenuPosition.x, createButton);
		createButton.onClick = [=]()
		{
			GameObject* pNewObject{ SceneManager::GetInstance().GetCurrentScene()->CreateGameObject("GameObject") };
			Editor::GetInstance().SetSelectedObject(pNewObject);
		};
		m_ControlButtons.emplace_back(createButton);
	}
}

void that::HierarchyWindow::OnMouseButton(int mouseButton, bool released, const glm::ivec2& point)
{
	if (released) return;

	m_ShowControlMenu = false;
	m_ShowNewMenu = false;

	if (mouseButton == 0)
	{
		// Delegate the click to all the buttons
		for (const auto& button : m_ControlButtons)
		{
			if (button.TryClick(point)) return;
		}
		for (const auto& button : m_Buttons)
		{
			if (button.TryClick(point)) return;
		}
	}
	else if (mouseButton == 1)
	{
		m_MenuPosition = point;
		// Delegate the click to all the buttons
		for (const auto& button : m_Buttons)
		{
			if (button.TryAltClick(point)) return;
		}

		m_ShowNewMenu = true;
	}
}

void that::HierarchyWindow::RenderObject(SDL_Renderer* pWindow, GameObject* pGameObject, int& curY, const std::string& curSpacing)
{
	// Create a new button and render it
	Button typeButton{};
	EditorGUI::GetInstance().RenderButton(pWindow, curSpacing + pGameObject->GetName(), curY, typeButton);
	typeButton.onClick = [=]()
	{
		Editor::GetInstance().SetSelectedObject(pGameObject);
	};
	typeButton.onAltClick = [=]()
	{
		m_ShowControlMenu = true;
		m_pControllingGameObject = pGameObject;
	};
	m_Buttons.emplace_back(typeButton);

	// Get all the children of this gameobject
	const auto& pChildren{ pGameObject->GetChildren() };

	// Stop here if there are no children to this gameobject
	if (pChildren.empty()) return;

	// Add a small space between a gameobject and its children
	curY += 2;

	// Render each child
	const std::string childSpacing{ curSpacing + "   " };
	for (const auto& pObject : pChildren)
	{
		RenderObject(pWindow, pObject, curY, childSpacing);

		curY += 5;
	}

}
