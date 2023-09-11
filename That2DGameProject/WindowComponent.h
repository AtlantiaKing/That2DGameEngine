#pragma once

struct SDL_Renderer;

#include "glm/vec2.hpp"

namespace that
{
	class WindowComponent
	{
	public:
		WindowComponent() = default;
		virtual ~WindowComponent() = default;

		WindowComponent(const WindowComponent& other) = delete;
		WindowComponent(WindowComponent&& other) = delete;
		WindowComponent& operator=(const WindowComponent& other) = delete;
		WindowComponent& operator=(WindowComponent&& other) = delete;

		virtual void Render(SDL_Renderer*) {}
		virtual void OnMouseButton(int, bool, const glm::ivec2&) {}
		virtual void OnMouseMovement(const glm::ivec2&) {}
	};
}