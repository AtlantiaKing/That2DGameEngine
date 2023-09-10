#include "HierarchyWindow.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "InspectorWindow.h"

#include "Editor.h"

void that::HierarchyWindow::Render(SDL_Renderer* pWindow)
{
	// Get all the objects in the scene
	const auto& pObjects{ SceneManager::GetInstance().GetCurrentScene()->GetObjects() };

	const std::string defaultSpacing{ "" };

	EditorGUI& gui{ EditorGUI::GetInstance() };
	gui.Begin(pWindow);

	// Render each gameobject
	for (const auto& pObject : pObjects)
	{
		RenderObject(pObject, defaultSpacing);

		// Add spacing between the gameobjects
		gui.Space(5);
	}

	if (m_ShowControlMenu)
	{
		gui.Move(m_MenuPosition.y);

		if (gui.RenderButton("Delete", m_MenuPosition.x))
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

			m_ShowControlMenu = false;
		}


		if (gui.RenderButton("Create child GameObject", m_MenuPosition.x))
		{
			GameObject* pNewObject{ m_pControllingGameObject->CreateGameObject("Child GameObject") };
			Editor::GetInstance().SetSelectedObject(pNewObject);

			m_ShowControlMenu = false;
		}
	}
	else if (m_ShowNewMenu)
	{
		gui.Move(m_MenuPosition.y);

		if (gui.RenderButton("Create new GameObject", m_MenuPosition.x))
		{
			GameObject* pNewObject{ SceneManager::GetInstance().GetCurrentScene()->CreateGameObject("GameObject") };
			Editor::GetInstance().SetSelectedObject(pNewObject);

			m_ShowNewMenu = false;
		}
	}

	if (gui.HasClickedWindow())
	{
		m_ShowControlMenu = false;
		m_ShowNewMenu = false;
	}
	if (gui.HasAltClickedWindow())
	{
		m_ShowNewMenu = true;
		m_MenuPosition = gui.GetCurserPosition();
	}
}

void that::HierarchyWindow::RenderObject(GameObject* pGameObject, const std::string& curSpacing)
{
	// Create a new button and render it
	Button typeButton{};
	int button{ EditorGUI::GetInstance().RenderMultiButton(curSpacing + pGameObject->GetName()) };
	if (button == 0)
	{
		Editor::GetInstance().SetSelectedObject(pGameObject);
	}
	else if (button == 1)
	{
		m_ShowControlMenu = true;
		m_MenuPosition = EditorGUI::GetInstance().GetCurserPosition();
		m_pControllingGameObject = pGameObject;
	}

	// Get all the children of this gameobject
	const auto& pChildren{ pGameObject->GetChildren() };

	// Stop here if there are no children to this gameobject
	if (pChildren.empty()) return;

	// Add a small space between a gameobject and its children
	EditorGUI::GetInstance().Space(2);

	// Render each child
	const std::string childSpacing{ curSpacing + "   " };
	for (const auto& pObject : pChildren)
	{
		RenderObject(pObject,  childSpacing);

		EditorGUI::GetInstance().Space(5);
	}
}
