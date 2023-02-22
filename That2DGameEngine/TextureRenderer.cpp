#include "TextureRenderer.h"
#include "Renderer.h"
#include "Transform.h"

that::TextureRenderer::TextureRenderer()
	: m_pTransform{ GetComponent<Transform>() }
{
}

void that::TextureRenderer::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void that::TextureRenderer::Render() const
{
	if (m_pTransform.expired()) return;

	auto pTransform{ m_pTransform.lock() };

	const auto& pos = pTransform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}
