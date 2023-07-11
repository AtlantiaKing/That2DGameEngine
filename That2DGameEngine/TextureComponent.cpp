#include "TextureComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "RenderComponent.h"

void that::TextureComponent::Render() const
{
	Texture2D* pTexture{ GetTexture() };

	// If no texture has been assigned to this TextureComponent, do nothing
	if (!pTexture) return;

	// Render the current texture at the position defined by the transform
	const glm::vec2& pos = GetTransform()->GetWorldPosition();
	const float rotation = GetTransform()->GetWorldRotation();
	const auto& scale{ GetTransform()->GetWorldScale() };

	Renderer::GetInstance().RenderTexture(
		*pTexture, GetConstantSource(),
		pos.x,
		pos.y,
		scale.x, scale.y,
		rotation,
		GetPivot(),
		true); // This true makes sure that the texture is rendered in camera space
}
