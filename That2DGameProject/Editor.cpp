#include "Editor.h"

#include "InspectorWindow.h"
#include "SceneWindow.h"
#include "HierarchyWindow.h"
#include "WindowPosition.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"

#include "EngineComponents.h"

#include "FileScene.h"
#include "TestScene.h"

that::Editor* that::Editor::m_pInstance{};

that::Editor::Editor(const std::string& dataPath)
{
	m_pInstance = this;

	that::ResourceManager::GetInstance().Init(dataPath);

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	auto& sceneManager{ SceneManager::GetInstance() };

	sceneManager.AddScene(digdug::FileScene::Load);
	sceneManager.LoadScene(0);

	StandaloneWindow inspectorWindow{ "Inspector", { 400, 400 } };
	auto pInspector{ inspectorWindow.AddComponent<InspectorWindow>() };
	auto pInspectorPosition{ inspectorWindow.AddComponent<WindowPosition>() };
	pInspectorPosition->SetWindow(inspectorWindow);
	pInspectorPosition->Move(-500, 0);
	m_Windows.emplace_back(std::move(inspectorWindow));

	StandaloneWindow hierarchyWindow{ "Hierarchy", { 400, 400 } };
	hierarchyWindow.AddComponent<HierarchyWindow>()->SetInspector(pInspector);
	auto pHierarchyPosition{ hierarchyWindow.AddComponent<WindowPosition>() };
	pHierarchyPosition->SetWindow(hierarchyWindow);
	pHierarchyPosition->Move(500, 0);
	m_Windows.emplace_back(std::move(hierarchyWindow));

	StandaloneWindow sceneView{ "Scene", { 600, 400 } };
	sceneView.AddComponent<SceneWindow>();
	m_Windows.emplace_back(std::move(sceneView));

	while (m_Windows.size() > 0)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT)
			{
				if(e.window.event == SDL_WINDOWEVENT_CLOSE) QuitWindow(e.window.windowID);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT) 
				{
					ClickWindow(e.button.windowID, { e.button.x, e.button.y });
				}
			}

			sceneManager.OnFrameStart();
			sceneManager.Update();
			sceneManager.LateUpdate();
			UpdateVisuals();
		}
	}
}

that::Editor::~Editor()
{
	Renderer::GetInstance().Destroy();

	SDL_Quit();
}

void that::Editor::UpdateVisuals()
{
	if (!m_pInstance) return;

	m_pInstance->UpdateVisualsInternal();
}

void that::Editor::UpdateVisualsInternal()
{
	for (const auto& window : m_Windows)
	{
		window.Render();
	}
}

void that::Editor::QuitWindow(Uint32 windowId)
{
	m_Windows.erase(std::find_if(begin(m_Windows), end(m_Windows), 
		[windowId](const auto& window) 
		{ 
			return window.IsId(windowId); 
		}
	));
}

void that::Editor::ClickWindow(Uint32 windowId, const glm::ivec2& point)
{
	std::find_if(begin(m_Windows), end(m_Windows), [windowId](const auto& window) { return window.IsId(windowId); })->Click(point);
}

