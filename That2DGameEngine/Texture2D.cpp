#include "Texture2D.h"

#include <SDL.h>

that::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 that::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* that::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

that::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_texture = texture;
}
