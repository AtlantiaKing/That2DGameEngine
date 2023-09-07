#pragma once

#include "WindowComponent.h"

#include "glm/vec2.hpp"

struct SDL_Window;

namespace that
{
	class GameObject;
	class StandaloneWindow;

	class WindowPosition final : public WindowComponent
	{
	public:
		WindowPosition() = default;
		virtual ~WindowPosition() = default;

		void SetWindow(StandaloneWindow& window);
		void SetPosition(const glm::ivec2& position) const;
		void SetPosition(int x, int y) const;
		void Move(const glm::ivec2& displacement) const;
		void Move(int x, int y) const;

	private:
		SDL_Window* m_pWindow{};
	};
}
