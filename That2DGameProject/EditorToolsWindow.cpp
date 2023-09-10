#include "EditorToolsWindow.h"

#include "Editor.h"

#include "InspectorWindow.h"
#include "WindowPosition.h"
#include "HierarchyWindow.h"
#include "SceneWindow.h"

void that::EditorToolsWindow::Render(SDL_Renderer* pWindow)
{
	m_Buttons.clear();

	EditorGUI& gui{ EditorGUI::GetInstance() };
	Button button{};

	gui.RenderImage(pWindow, "Editor/Inspector.png", 0, 0, 0.0f, 0.0f, button);
	button.onClick = []()
	{
		Editor& editor{ Editor::GetInstance() };

		if (editor.HasWindowWithComponent<InspectorWindow>()) return;

		StandaloneWindow* pWindow{ editor.CreateWindow("Inspector", { 400, 400 }) };
		pWindow->AddComponent<InspectorWindow>();
		auto pInspectorPosition{ pWindow->AddComponent<WindowPosition>() };
		pInspectorPosition->SetWindow(*pWindow);
		pInspectorPosition->Move(-500, 0);
	};
	m_Buttons.emplace_back(button);

	gui.RenderImage(pWindow, "Editor/SceneView.png", 50, 0, 0.0f, 0.0f, button);
	button.onClick = []()
	{
		Editor& editor{ Editor::GetInstance() };

		if (editor.HasWindowWithComponent<SceneWindow>()) return;

		StandaloneWindow* pWindow{ editor.CreateWindow("Scene", { 600, 400 }) };
		pWindow->AddComponent<SceneWindow>();
	};
	m_Buttons.emplace_back(button);

	gui.RenderImage(pWindow, "Editor/Hierarchy.png", 100, 0, 0.0f, 0.0f, button);
	button.onClick = []()
	{
		Editor& editor{ Editor::GetInstance() };

		if (editor.HasWindowWithComponent<HierarchyWindow>()) return;

		StandaloneWindow* pWindow{ editor.CreateWindow("Hierarchy", { 400, 400 }) };
		pWindow->AddComponent<HierarchyWindow>();
		auto pInspectorPosition{ pWindow->AddComponent<WindowPosition>() };
		pInspectorPosition->SetWindow(*pWindow);
		pInspectorPosition->Move(500, 0);
	};
	m_Buttons.emplace_back(button);
}

void that::EditorToolsWindow::OnMouseButton(int mouseButton, bool released, const glm::ivec2& point)
{
	if (mouseButton != 0 || released) return;

	for (const auto& button : m_Buttons)
	{
		button.TryClick(point);
	}
}
