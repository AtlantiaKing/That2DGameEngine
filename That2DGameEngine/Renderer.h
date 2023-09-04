#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "glm/vec2.hpp"

namespace that
{
	class Texture2D;
	class CameraComponent;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_pRenderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, bool useCamera = true) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float scaleX, float scaleY, float rotation, bool useCamera = true) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y, bool useCamera = true) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y, float scaleX, float scaleY, float rotation, const glm::vec2& pivot = { 0.5f, 0.5f }, bool useCamera = true) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, SDL_Rect dstRect, float rotation, bool useCamera = true) const;

		void DrawRect(SDL_Rect rect, const SDL_Color& color, bool useCamera = false);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		void SetCamera(CameraComponent* pCamera);
		CameraComponent* GetCamera() const { return m_pCamera; }

	private:
		CameraComponent* m_pCamera{};

		glm::vec2 m_RenderPosition{};
	};
}