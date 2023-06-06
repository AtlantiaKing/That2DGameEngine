#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct SDL_Surface;
namespace that
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Surface2D final
	{
	public:
		SDL_Surface* GetSDLSurface() const;
		explicit Surface2D(SDL_Surface* pSurface);
		~Surface2D();

		glm::ivec2 GetSize() const;
		glm::ivec4 GetPixel(int x, int y) const;

		Surface2D(const Surface2D&) = delete;
		Surface2D(Surface2D&&) = delete;
		Surface2D& operator= (const Surface2D&) = delete;
		Surface2D& operator= (const Surface2D&&) = delete;
	private:
		SDL_Surface* m_pSurface;
	};
}