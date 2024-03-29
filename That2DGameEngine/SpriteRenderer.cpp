#include "SpriteRenderer.h"

#include "Transform.h"

#include "Renderer.h"
#include "Timer.h"

void that::SpriteRenderer::SetSprite(std::shared_ptr<Texture2D> pTexture, int tilesInX, int tilesInY, float timePerTile)
{
	m_pTexture = pTexture;

	m_NrTilesX = tilesInX;
	m_NrTiles = tilesInX * tilesInY;
	m_TimeBetweenTiles = timePerTile;

	Reset();

	RecalculateSrcRect();
}

void that::SpriteRenderer::SetTexture(std::shared_ptr<Texture2D> pTexture, bool reset)
{
	m_pTexture = pTexture;

	if(reset) Reset();
}

void that::SpriteRenderer::SetTimePerTile(float timePerTile, bool reset)
{
	m_TimeBetweenTiles = timePerTile;

	if (reset)
	{
		Reset();
		RecalculateSrcRect();
	}
}

void that::SpriteRenderer::SetTile(int tile)
{
	m_CurTile = tile % m_NrTiles;

	RecalculateSrcRect();
}

glm::ivec2 that::SpriteRenderer::GetTextureSize() const
{
	return { m_SrcRect.w, m_SrcRect.h };
}

glm::vec2 that::SpriteRenderer::GetScaledTextureSize() const
{
	const auto& scale{ GetTransform()->GetWorldScale() };

	return { m_SrcRect.w * scale.x, m_SrcRect.h * scale.y };
}

int that::SpriteRenderer::GetTile() const
{
	return m_CurTile;
}

void that::SpriteRenderer::Update()
{
	if (m_NrTiles == 0) return;

	m_CurTime += that::Timer::GetInstance().GetElapsed();
	if (m_CurTime > m_TimeBetweenTiles)
	{
		m_CurTime -= m_TimeBetweenTiles;
		++m_CurTile %= m_NrTiles;

		RecalculateSrcRect();
	}
}

void that::SpriteRenderer::RecalculateSrcRect()
{
	m_SrcRect.x = m_CurTile % m_NrTilesX * m_SrcRect.w;
	m_SrcRect.y = m_CurTile / m_NrTilesX * m_SrcRect.h;
}

void that::SpriteRenderer::Render() const
{
	// If no texture has been assigned to this SpriteRenderer, do nothing
	if (!m_pTexture) return;

	// Render the current texture at the position defined by the transform
	const glm::vec2& pos = GetTransform()->GetWorldPosition();
	const float rotation = GetTransform()->GetWorldRotation();
	const auto& scale{ GetTransform()->GetWorldScale() };

	const SDL_Rect dstRect
	{
		static_cast<int>(pos.x - m_SrcRect.w * abs(scale.x) / 2),
		static_cast<int>(pos.y - m_SrcRect.h * abs(scale.y) / 2),
		static_cast<int>(m_SrcRect.w * scale.x),
		static_cast<int>(m_SrcRect.h * scale.y)
	};

	Renderer::GetInstance().RenderTexture(
		*m_pTexture, m_SrcRect,
		dstRect,
		rotation);
}

void that::SpriteRenderer::Reset()
{
	m_CurTime = 0.0f;
	m_CurTile = 0;

	const auto& textureSize{ m_pTexture->GetSize() };
	const int nrTilesY{ m_NrTiles / m_NrTilesX };

	m_SrcRect.w = textureSize.x / m_NrTilesX;
	m_SrcRect.h = textureSize.y / nrTilesY;
}
