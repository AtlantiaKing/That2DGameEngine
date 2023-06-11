#include "Window.h"

#include <SDL.h>

#include "Renderer.h"

const glm::ivec2& that::Window::GetSize() const
{
	return m_WindowSize;
}

bool that::Window::SetTitle(const std::string& title)
{
	// Cache the window title
	m_WindowTitle = title;

	// Rebuild the window
	return Rebuild();
}

const std::string& that::Window::GetTitle() const
{
	return m_WindowTitle;
}

bool that::Window::SetWindowSize(int width, int height)
{
	// Cache the window size
	m_WindowSize.x = width;
	m_WindowSize.y = height;

	// Rebuild the window
	return Rebuild();
}

void that::Window::Destroy()
{
	// Don't release a window that is a nullptr
	if (!m_pWindow) return;

	// Destroy and reset the window
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
}

bool that::Window::Rebuild()
{
	// Create a new SDL window
	SDL_Window* pWindow{ SDL_CreateWindow(
		m_WindowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_WindowSize.x,
		m_WindowSize.y,
		SDL_WINDOW_OPENGL
	) };

	// If the window creation failed, return fail result
	if (!pWindow) return false;

	// At this point a new window is succesfully created

	// Destroy the current window
	Destroy();
	// Set the new window
	m_pWindow = pWindow;

	// Initialize the renderer
	Renderer::GetInstance().Init(m_pWindow);

	// Return a succes result
	return true;
}

that::Window::~Window()
{
	// Release window data
	Destroy();
}
