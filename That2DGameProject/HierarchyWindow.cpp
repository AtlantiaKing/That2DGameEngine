#include "HierarchyWindow.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "InspectorWindow.h"

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

		Button deleteButton{};
		EditorGUI::GetInstance().RenderButton(pWindow, "Delete", curStartY, m_MenuPosition.x, deleteButton);
		deleteButton.onClick = [=]()
		{
			m_pControllingGameObject->GetScene()->DestroyInstant(m_pControllingGameObject);
		};
		m_ControlButtons.emplace_back(deleteButton);
	}
	else if (m_ShowNewMenu)
	{
		curStartY = m_MenuPosition.y;

		Button createButton{};
		EditorGUI::GetInstance().RenderButton(pWindow, "Create new GameObject", curStartY, m_MenuPosition.x, createButton);
		createButton.onClick = [=]()
		{
			GameObject* pNewObject{ SceneManager::GetInstance().GetCurrentScene()->CreateGameObject("GameObject") };
			m_pInspector->SetGameObject(pNewObject);
		};
		m_ControlButtons.emplace_back(createButton);
	}

	m_ShowControlMenu = false;
	m_ShowNewMenu = false;
}

void that::HierarchyWindow::OnClick(const glm::ivec2& point)
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

void that::HierarchyWindow::OnAltClick(const glm::ivec2& point)
{
	m_MenuPosition = point;
	// Delegate the click to all the buttons
	for (const auto& button : m_Buttons)
	{
		if (button.TryAltClick(point)) return;
	}

	m_ShowNewMenu = true;
}

void that::HierarchyWindow::RenderObject(SDL_Renderer* pWindow, GameObject* pGameObject, int& curY, const std::string& curSpacing)
{
	// Create a new button and render it
	Button typeButton{};
	EditorGUI::GetInstance().RenderButton(pWindow, curSpacing + pGameObject->GetName(), curY, typeButton);
	typeButton.onClick = [=]()
	{
		m_pInspector->SetGameObject(pGameObject);
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
