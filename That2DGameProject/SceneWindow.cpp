#include "SceneWindow.h"

#include "Renderer.h"
#include "SceneManager.h"

void that::SceneWindow::Render(SDL_Renderer* pWindow)
{
	if (!m_Initialised)
	{
		m_Initialised = true;
		Renderer::GetInstance().Init(SDL_RenderGetWindow(pWindow), pWindow);
	}

	SceneManager::GetInstance().Render();
}
