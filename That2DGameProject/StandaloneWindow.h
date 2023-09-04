#pragma once

#include <SDL.h>

#include "glm/vec2.hpp"

#include <string>

namespace that
{
	class StandaloneWindow final
	{
	public:
		StandaloneWindow(const std::string& name, const glm::ivec2& size);
		~StandaloneWindow();

		StandaloneWindow(const StandaloneWindow& other) = delete;
		StandaloneWindow(StandaloneWindow&& other) noexcept;
		StandaloneWindow& operator=(const StandaloneWindow& other) = delete;
		StandaloneWindow& operator=(StandaloneWindow&& other) noexcept;

		bool IsValid() const;
		bool IsId(Uint32 id) const;

		void Render() const;
	private:
		static int GetOpenGLDriverIndex();

		static SDL_Color m_Background;

		SDL_Window* m_pWindow{};
		SDL_Renderer* m_pRenderer{};
	};
}
