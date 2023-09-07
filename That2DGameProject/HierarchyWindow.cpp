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
	m_Buttons.clear();

	const auto& pObjects{ SceneManager::GetInstance().GetCurrentScene()->GetObjects() };

	int curStartY{};
	std::string defaultSpacing{ "" };

	for (const auto& pObject : pObjects)
	{
		RenderObject(pWindow, pObject, curStartY, defaultSpacing);

		curStartY += 5;
	}
}

void that::HierarchyWindow::OnClick(const glm::ivec2& point)
{
	for (const auto& button : m_Buttons)
	{
		button.TryClick(point);
	}
}

void that::HierarchyWindow::RenderObject(SDL_Renderer* pWindow, GameObject* pGameObject, int& curY, const std::string& curSpacing)
{
	Button typeButton{};
	EditorGUI::GetInstance().RenderButton(pWindow, curSpacing + pGameObject->GetName(), curY, typeButton);
	typeButton.onClick = [=]()
	{
		m_pInspector->SetGameObject(pGameObject);
	};

	curY += 2;

	const auto& pChildren{ pGameObject->GetChildren() };

	std::string childSpacing{ curSpacing + "   " };
	for (const auto& pObject : pChildren)
	{
		RenderObject(pWindow, pObject, curY, childSpacing);

		curY += 5;
	}

	m_Buttons.emplace_back(typeButton);
}
