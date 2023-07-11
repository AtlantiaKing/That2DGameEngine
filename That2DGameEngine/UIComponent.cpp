#include "UIComponent.h"
#include "Renderer.h"
#include "Transform.h"

void that::UIComponent::Render() const
{
	Texture2D* pTexture{ GetTexture() };

	// If no texture has been assigned to this UIComponent, do nothing
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
		false); // This false makes sure that the texture is rendered in screen space instead of camera space
}