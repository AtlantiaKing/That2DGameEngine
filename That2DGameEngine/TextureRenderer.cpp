#include "TextureRenderer.h"
#include "Transform.h"

void that::TextureRenderer::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void that::TextureRenderer::SetPivot(const glm::vec2& pivot)
{
	m_Pivot = pivot;
}

void that::TextureRenderer::SetPivot(float x, float y)
{
	m_Pivot.x = x;
	m_Pivot.y = y;
}

glm::vec2 that::TextureRenderer::GetScaledTextureSize(Transform* pTransform) const
{
	const auto& texSize{ m_pTexture->GetSize() };
	const auto& scale{ pTransform->GetWorldScale() };

	return
	{
		texSize.x * scale.x,
		texSize.y * scale.y
	};
}