#pragma once

#include "WindowComponent.h"

#include <SDL.h>

#include "glm/vec2.hpp"

#include <string>
#include <vector>
#include <memory>

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

		template<typename T>
		void AddComponent()
		{
			static_assert(std::is_base_of<WindowComponent, T>());

			m_pComponents.emplace_back(std::make_unique<T>());
		}

		void Render() const;
	private:
		static int GetOpenGLDriverIndex();

		static SDL_Color m_Background;

		SDL_Window* m_pWindow{};
		SDL_Renderer* m_pRenderer{};

		std::vector<std::unique_ptr<WindowComponent>> m_pComponents{};
	};
}
