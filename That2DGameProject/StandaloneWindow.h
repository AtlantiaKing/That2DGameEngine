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
		T* AddComponent()
		{
			static_assert(std::is_base_of<WindowComponent, T>());

			std::unique_ptr<T> pComponent{ std::make_unique<T>() };
			T* pRawComponent{ pComponent.get() };

			m_pComponents.emplace_back(std::move(pComponent));

			return pRawComponent;
		}

		template<typename T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of<WindowComponent, T>());

			for (const auto& pComponent : m_pComponents)
			{
				T* derivedComponent{ dynamic_cast<T*>(pComponent.get()) };

				if (derivedComponent) return derivedComponent;
			}

			return nullptr;
		}

		void Render() const;
		void Click(const glm::ivec2& point) const;
		void AltClick(const glm::ivec2& point) const;

		SDL_Window* GetSDLWindow() const { return m_pWindow; }

	private:
		static int GetOpenGLDriverIndex();

		static SDL_Color m_Background;

		SDL_Window* m_pWindow{};
		SDL_Renderer* m_pRenderer{};

		std::vector<std::unique_ptr<WindowComponent>> m_pComponents{};
	};
}
