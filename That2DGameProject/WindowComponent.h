#pragma once

struct SDL_Renderer;

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

		virtual void Render(SDL_Renderer* pWindow) const = 0;
	};
}
