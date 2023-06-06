#include "TextureRenderer.h"
#include "Renderer.h"
#include "Transform.h"

void that::TextureRenderer::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
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
	const auto& textureSize{ m_pTexture->GetSize() };

	Renderer::GetInstance().RenderTexture(
		*m_pTexture, m_SrcRect, 
		pos.x - textureSize.x * abs(scale.x) / 2.0f,
		pos.y - textureSize.y * abs(scale.y) / 2.0f,
		scale.x, scale.y,
		rotation);
}
