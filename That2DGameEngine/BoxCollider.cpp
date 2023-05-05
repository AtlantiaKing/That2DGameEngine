#include "BoxCollider.h"

#include "GameObject.h"

#include "TextureRenderer.h"

void that::BoxCollider::Init()
{
	// Add the collider to the physics system
	Physics::GetInstance().AddCollider(this);

	// Don't overwrite settings by the user
	if (!m_DefaultSize) return;

	// Set the size of the collider to the size of the texture if there is a TextureRenderer
	TextureRenderer* pTexture{ GetOwner()->GetComponent<TextureRenderer>() };
	if (pTexture)
	{
		m_Size = pTexture->GetScaledTextureSize();
	}
}

void that::BoxCollider::OnDestroy()
{
	// Remove the collider from the physics system
	Physics::GetInstance().RemoveCollider(this);
}

void that::BoxCollider::SetSize(const glm::vec2 size)
{
	SetSize(size.x, size.y);
}

void that::BoxCollider::SetSize(float x, float y)
{
	m_Size.x = x;
	m_Size.y = y;
	m_DefaultSize = false;
}

void that::BoxCollider::Hit(BoxCollider* pOther)
{
	// Create a collision data object
	CollisionData collision
	{
		this,
		pOther
	};

	// Notify all observers
	m_OnHitEvent.Notify(collision);
}
