#include "Surface2D.h"

#include <SDL.h>

SDL_Surface* that::Surface2D::GetSDLSurface() const
{
    return nullptr;
}

that::Surface2D::Surface2D(SDL_Surface* pSurface)
    :m_pSurface { pSurface }
{
}

that::Surface2D::~Surface2D()
{
    SDL_FreeSurface(m_pSurface);
}

glm::ivec2 that::Surface2D::GetSize() const
{
    return { m_pSurface->w, m_pSurface->h };
}

glm::ivec4 that::Surface2D::GetPixel(int x, int y) const
{
    Uint8* pixel = (Uint8*)m_pSurface->pixels + y * m_pSurface->pitch + x * sizeof(Uint32);

    return { pixel[0], pixel[1], pixel[2], pixel[3] };
}
