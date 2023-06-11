#include "TextureRenderer.h"
#include "Renderer.h"
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

glm::vec2 that::TextureRenderer::GetScaledTextureSize() const
{
	const auto& texSize{ m_pTexture->GetSize() };
	const auto& scale{ GetTransform()->GetWorldScale() };

	return
	{
		texSize.x * scale.x,
		texSize.y * scale.y
	};
}

void that::TextureRenderer::Render() const
{
	// If no texture has been assigned to this TextureRenderer, do nothing
	if (!m_pTexture) return;

	// Render the current texture at the position defined by the transform
	const glm::vec2& pos = GetTransform()->GetWorldPosition();
	const float rotation = GetTransform()->GetWorldRotation();
	const auto& scale{ GetTransform()->GetWorldScale() };

	Renderer::GetInstance().RenderTexture(
		*m_pTexture, m_SrcRect, 
		pos.x,
		pos.y,
		scale.x, scale.y,
		rotation,
		m_Pivot);
}
