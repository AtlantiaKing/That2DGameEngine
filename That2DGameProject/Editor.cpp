#include "Editor.h"

#include "HierarchyWindow.h"
#include "ResourceManager.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "EngineComponents.h"

that::Editor* that::Editor::m_pInstance{};

that::Editor::Editor(const std::string& dataPath)
{
	that::ResourceManager::GetInstance().Init(dataPath);

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pInstance = this;

	auto hierarchy{ StandaloneWindow{ "Hierarchy", { 400, 400 } } };
	hierarchy.AddComponent<HierarchyWindow>();
	m_Windows.emplace_back(std::move(hierarchy));

	std::unique_ptr<GameObject> m_pObject{ std::make_unique<GameObject>(nullptr, "Super Cool GameObject") };
	m_Windows[0].GetComponent<HierarchyWindow>()->SetGameObject(m_pObject.get());

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

			UpdateVisuals();
		}
	}
}

that::Editor::~Editor()
{
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
	m_Windows.erase(std::remove_if(begin(m_Windows), end(m_Windows), 
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

