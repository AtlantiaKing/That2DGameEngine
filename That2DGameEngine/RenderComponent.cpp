#include "RenderComponent.h"

void that::RenderComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_Texture.SetTexture(pTexture);
}

void that::RenderComponent::SetPivot(const glm::vec2& pivot)
{
	m_Texture.SetPivot(pivot);
}

void that::RenderComponent::SetPivot(float x, float y)
{
	m_Texture.SetPivot(x, y);
}

glm::ivec2 that::RenderComponent::GetTextureSize() const
{
	return m_Texture.GetTextureSize();
}

glm::vec2 that::RenderComponent::GetScaledTextureSize() const
{
	return m_Texture.GetScaledTextureSize(GetTransform());
}

SDL_Rect& that::RenderComponent::GetSource()
{
	return m_Texture.GetSource();
}

const glm::vec2& that::RenderComponent::GetPivot() const
{
	return m_Texture.GetPivot();
}

const SDL_Rect& that::RenderComponent::GetConstantSource() const
{
	return m_Texture.GetConstantSource();
}

that::Texture2D* that::RenderComponent::GetTexture() const
{
	return m_Texture.GetTexture();
}