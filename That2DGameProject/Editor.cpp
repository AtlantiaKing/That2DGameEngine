#include "Editor.h"

#include "InspectorWindow.h"
#include "SceneWindow.h"
#include "HierarchyWindow.h"
#include "WindowPosition.h"
#include "EditorToolsWindow.h"

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
#include <SceneSerialization.h>

void that::Editor::Init(const std::string& dataPath)
{
	// Initialize the resource manager
	that::ResourceManager::GetInstance().Init(dataPath);

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	// Start the first scene
	auto& sceneManager{ SceneManager::GetInstance() };
	sceneManager.AddScene(digdug::FileScene::Load);
	sceneManager.LoadScene(0);

	// WINDOWS
	// Create scene view window and move it to the top
	StandaloneWindow editorTools{ "Editor Tools", { 300, 50 } };
	editorTools.AddComponent<EditorToolsWindow>();
	auto pToolsPosition{ editorTools.AddComponent<WindowPosition>() };
	pToolsPosition->SetWindow(editorTools);
	pToolsPosition->Move(0, -275);
	m_ToolsWindow = SDL_GetWindowID(editorTools.GetSDLWindow());
	m_Windows.emplace_back(std::move(editorTools));

	// Create inspector window and move it to the left
	StandaloneWindow inspectorWindow{ "Inspector", { 400, 400 } };
	auto pInspector{ inspectorWindow.AddComponent<InspectorWindow>() };
	auto pInspectorPosition{ inspectorWindow.AddComponent<WindowPosition>() };
	pInspectorPosition->SetWindow(inspectorWindow);
	pInspectorPosition->Move(-500, 0);
	m_Windows.emplace_back(std::move(inspectorWindow));

	// Create hierarchy window and move it to the right
	StandaloneWindow hierarchyWindow{ "Hierarchy", { 400, 400 } };
	hierarchyWindow.AddComponent<HierarchyWindow>()->SetInspector(pInspector);
	auto pHierarchyPosition{ hierarchyWindow.AddComponent<WindowPosition>() };
	pHierarchyPosition->SetWindow(hierarchyWindow);
	pHierarchyPosition->Move(500, 0);
	m_Windows.emplace_back(std::move(hierarchyWindow));

	// Create scene view window and keep it in the center
	StandaloneWindow sceneView{ "Scene", { 600, 400 } };
	sceneView.AddComponent<SceneWindow>();
	m_Windows.emplace_back(std::move(sceneView));

	// As long as there are windows available
	while (m_Windows.size() > 0)
	{
		// As long as there are SDL events in the queue
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			bool nextEvent{};

			// Delegate SDL events to their respective window
			switch (e.type)
			{
			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_CLOSE) QuitWindow(e.window.windowID);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					MouseButtonWindow(e.button.windowID, 0, false, { e.button.x, e.button.y });
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					MouseButtonWindow(e.button.windowID, 1, false, { e.button.x, e.button.y });
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				m_PrevMousePos = {};
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					MouseButtonWindow(e.button.windowID, 0, true, { e.button.x, e.button.y });
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					MouseButtonWindow(e.button.windowID, 1, true, { e.button.x, e.button.y });
				}
				break;
			}
			case SDL_MOUSEMOTION:
			{
				glm::ivec2 position{ e.motion.x, e.motion.y };
				glm::ivec2 displacement{};
				if (!(m_PrevMousePos.x == 0 && m_PrevMousePos.y == 0))
				{
					displacement.x = position.x - m_PrevMousePos.x;
					displacement.y = position.y - m_PrevMousePos.y;
				}

				MouseMovementWindow(e.motion.windowID, displacement);
				m_PrevMousePos = position;
				break;
			}
			default:
				nextEvent = true;
			}
			if (nextEvent) continue;
			
			// Update and render the scene when a SDL event gets triggered
			sceneManager.OnFrameStart();
			sceneManager.Update();
			sceneManager.LateUpdate();
			UpdateVisuals();
		}
	}

	reflection::SceneSerialization::SerializeScene(sceneManager.GetCurrentScene());
}

void that::Editor::Shutdown()
{
	m_Windows.clear();
}

that::StandaloneWindow* that::Editor::CreateWindow(const std::string& title, const glm::ivec2& size)
{
	StandaloneWindow window{ title, size };
	m_Windows.emplace_back(std::move(window));
	return &m_Windows[m_Windows.size() - 1];
}

that::Editor::~Editor()
{
	Renderer::GetInstance().Destroy();

	SDL_Quit();
}

void that::Editor::UpdateVisuals()
{
	for (const auto& window : m_Windows)
	{
		window.Render();
	}
}

void that::Editor::QuitWindow(Uint32 windowId)
{
	if (m_ToolsWindow == windowId)
	{
		Shutdown();
		return;
	}

	m_Windows.erase(std::find_if(begin(m_Windows), end(m_Windows), 
		[windowId](const auto& window) 
		{ 
			return window.IsId(windowId); 
		}
	));
}

void that::Editor::MouseButtonWindow(Uint32 windowId, int mouseButton, bool released, const glm::ivec2& point) const
{
	std::find_if(begin(m_Windows), end(m_Windows), [windowId](const auto& window) { return window.IsId(windowId); })->MouseButton(mouseButton, released, point);
}

void that::Editor::MouseMovementWindow(Uint32 windowId, const glm::ivec2& displacement) const
{
	std::find_if(begin(m_Windows), end(m_Windows), [windowId](const auto& window) { return window.IsId(windowId); })->MouseMovement(displacement);
}

