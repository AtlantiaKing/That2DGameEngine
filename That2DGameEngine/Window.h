#pragma once

#include "Singleton.h"

#include "glm/vec2.hpp"

struct SDL_Window;

namespace that
{
	class Window final : public Singleton<Window>
	{
	public:
		bool SetWindowSize(int width, int height);

		const glm::ivec2& GetSize() const;

	private:		
		friend class Singleton<Window>;
		Window() = default;
		~Window();

		void Destroy();

		glm::ivec2 m_WindowSize{};

		SDL_Window* m_pWindow{};
	};
}

