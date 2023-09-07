#include "WindowPosition.h"

#include "Logger.h"

#include "SDL_image.h"

#include "StandaloneWindow.h"

void that::WindowPosition::SetWindow(StandaloneWindow& window)
{
	m_pWindow = window.GetSDLWindow();
}

void that::WindowPosition::SetPosition(const glm::ivec2& position) const
{
	SetPosition(position.x, position.y);
}

void that::WindowPosition::SetPosition(int x, int y) const
{
	if (!m_pWindow)
	{
		Logger::LogWarning("Failed changing position of an editor window because there is none assigned");
		return;
	}
	SDL_SetWindowPosition(m_pWindow, x, y);
}

void that::WindowPosition::Move(const glm::ivec2& displacement) const
{
	Move(displacement.x, displacement.y);
}

void that::WindowPosition::Move(int x, int y) const
{
	if (!m_pWindow)
	{
		Logger::LogWarning("Failed moving an editor window because there is none assigned");
		return;
	}

	glm::ivec2 prevPosition{};
	SDL_GetWindowPosition(m_pWindow, &prevPosition.x, &prevPosition.y);

	SDL_SetWindowPosition(m_pWindow, prevPosition.x + x, prevPosition.y + y);
}
