#include "StandaloneWindow.h"

#include <stdexcept>

SDL_Color that::StandaloneWindow::m_Background{ 255, 255, 255, 255 };

that::StandaloneWindow::StandaloneWindow(const std::string& name, const glm::ivec2& size)
{
	// Create a new SDL window
	m_pWindow = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.x,
		size.y,
		SDL_WINDOW_OPENGL
	);

	// If the window creation failed, return fail result
	if (!m_pWindow) throw std::runtime_error("Failed to create a standalone window");

	m_pRenderer = SDL_CreateRenderer(m_pWindow, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (!m_pRenderer)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

that::StandaloneWindow::~StandaloneWindow()
{
	// Don't release a window that is a nullptr
	if (!m_pWindow) return;

	// Destroy and reset the window
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
}

that::StandaloneWindow::StandaloneWindow(StandaloneWindow&& other) noexcept
{
	m_pWindow = other.m_pWindow;
	m_pRenderer = other.m_pRenderer;
	other.m_pWindow = nullptr;
	other.m_pRenderer = nullptr;
}

that::StandaloneWindow& that::StandaloneWindow::operator=(StandaloneWindow&& other) noexcept
{
	m_pWindow = other.m_pWindow;
	m_pRenderer = other.m_pRenderer;
	other.m_pWindow = nullptr;
	other.m_pRenderer = nullptr;

	return *this;
}

bool that::StandaloneWindow::IsValid() const
{
	return m_pWindow != nullptr;
}

bool that::StandaloneWindow::IsId(Uint32 id) const
{
	return SDL_GetWindowID(m_pWindow) == id;
}

void that::StandaloneWindow::Render() const
{
	const auto& color = m_Background;
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

int that::StandaloneWindow::GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}
