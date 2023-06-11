#pragma once

#include "Singleton.h"

#include "glm/vec2.hpp"

#include <string>

struct SDL_Window;

namespace that
{
	class Window final : public Singleton<Window>
	{
	public:
		bool SetWindowSize(int width, int height);
		const glm::ivec2& GetSize() const;

		bool SetTitle(const std::string& title);
		const std::string& GetTitle() const;

	private:		
		friend class Singleton<Window>;
		Window() = default;
		~Window();

		void Destroy();
		bool Rebuild();

		glm::ivec2 m_WindowSize{};
		std::string m_WindowTitle{ "That2DGameEngine Project" };

		SDL_Window* m_pWindow{};
	};
}

