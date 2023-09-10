#include "SceneWindow.h"

#include "Renderer.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"

#include "Editor.h"
#include "EditorGUI.h"

void that::SceneWindow::Render(SDL_Renderer* pWindow)
{
	if (!m_Initialised)
	{
		m_Initialised = true;
		Renderer::GetInstance().Init(SDL_RenderGetWindow(pWindow), pWindow);
	}

	SceneManager::GetInstance().Render();

	m_Buttons.clear();

	GameObject* pSelectedObj{ Editor::GetInstance().GetSelectedObject() };
	if (!pSelectedObj) return;

	EditorGUI& gui{ EditorGUI::GetInstance() };
	const glm::vec2& position{ pSelectedObj->GetTransform()->GetWorldPosition() + Renderer::GetInstance().GetRenderOffset() };

	Button xAxis{};
	gui.RenderImage(pWindow, "Editor/XAxis.png", static_cast<int>(position.x), static_cast<int>(position.y), 0.0f, 0.5f, xAxis);
	xAxis.onClick = [&]() { m_MoveX = true; };
	m_Buttons.push_back(xAxis);

	Button yAxis{};
	gui.RenderImage(pWindow, "Editor/YAxis.png", static_cast<int>(position.x), static_cast<int>(position.y), 0.5f, 1.0f, yAxis);
	yAxis.onClick = [&]() { m_MoveY = true; };
	m_Buttons.push_back(yAxis);

	gui.RenderImage(pWindow, "Editor/BothAxis.png", static_cast<int>(position.x), static_cast<int>(position.y), 0.0f, 1.0f);
	gui.RenderImage(pWindow, "Editor/SelectedObject.png", static_cast<int>(position.x), static_cast<int>(position.y), 0.5f, 0.5f);
}

void that::SceneWindow::OnMouseButton(int mouseButton, bool released, const glm::ivec2& point)
{
	if (mouseButton != 0) return;

	if (released)
	{
		m_MoveX = false;
		m_MoveY = false;
		return;
	}

	for (const auto& button : m_Buttons)
	{
		button.TryClick(point);
	}
}

void that::SceneWindow::OnMouseMovement(const glm::ivec2& displacement)
{
	if (!m_MoveX && !m_MoveY) return;

	GameObject* pCurObject{ Editor::GetInstance().GetSelectedObject() };
	glm::vec2 position{ pCurObject->GetTransform()->GetWorldPosition() };

	if (m_MoveX)
	{
		position.x += displacement.x;
	}
	if (m_MoveY)
	{
		position.y += displacement.t;
	}

	pCurObject->GetTransform()->SetWorldPosition(position);
}
