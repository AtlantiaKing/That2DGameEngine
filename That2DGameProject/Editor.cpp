#include "Editor.h"

#include "Engine.h"
#include <stdexcept>

that::Editor* that::Editor::m_pInstance{};

that::Editor::Editor(const std::string&)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pInstance = this;

	m_Windows.emplace_back(StandaloneWindow{ "Hierarchy", { 400, 400 } });
	m_Windows.emplace_back(StandaloneWindow{ "Project Management", { 400, 400 } });
	m_Windows.emplace_back(StandaloneWindow{ "Inspector", { 400, 400 } });

	while (m_Windows.size() > 0)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT)
			{
				if(e.window.event == SDL_WINDOWEVENT_CLOSE) QuitWindow(e.window.windowID);
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

