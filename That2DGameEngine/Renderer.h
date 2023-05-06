#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "TrashTheCache.h"

namespace that
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float scaleX, float scaleY, float rotation) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y, float scaleX, float scaleY, float rotation) const;

		void DrawRect(const SDL_Rect& rect, const SDL_Color& color);


		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

