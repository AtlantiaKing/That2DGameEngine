#include "TextureRenderer.h"
#include "Renderer.h"
#include "Transform.h"

void that::TextureRenderer::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void that::TextureRenderer::Render() const
{
	// Get the Transform on the parent; if the parent has no Transform, do nothing
	// TODO: Log a warning that no Transform is assigned to the parent of this TextureRenderer
	const auto pTransform{ GetComponent<Transform>() };
	if (!pTransform) return;

	// If no texture has been assigned to this TextureRenderer, do nothing
	if (!m_pTexture) return;

	// Render the current texture at the position defined by the transform
	const glm::vec2& pos = pTransform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
