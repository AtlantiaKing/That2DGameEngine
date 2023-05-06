#include "BoxCollider.h"

#include "GameObject.h"

#include "TextureRenderer.h"
#include "Transform.h"

#include "Renderer.h"

void that::BoxCollider::Init()
{
	// Add the collider to the physics system
	Physics::GetInstance().AddCollider(this);

	// Don't overwrite settings by the user
	if (m_DefaultSize)
	{
		// Set the size of the collider to the size of the texture if there is a TextureRenderer
		TextureRenderer* pTexture{ GetOwner()->GetComponent<TextureRenderer>() };
		if (pTexture) m_Size = pTexture->GetScaledTextureSize();
	}
}

void that::BoxCollider::OnDestroy()
{
	// Remove the collider from the physics system
	Physics::GetInstance().RemoveCollider(this);
}

void that::BoxCollider::Render() const
{
	const auto& center{ GetCenterWorld() };
	const auto& size{ GetSizeWorld() };

	SDL_Rect rect
	{
		static_cast<int>(center.x - size.x / 2.0f),
		static_cast<int>(center.y - size.y / 2.0f),
		static_cast<int>(size.x),
		static_cast<int>(size.y)
	};
	that::Renderer::GetInstance().DrawRect(rect, SDL_Color{ 0,255,0,255 });
}

void that::BoxCollider::SetCenter(const glm::vec2& center)
{
	SetCenter(center.x, center.y);
}

void that::BoxCollider::SetCenter(float x, float y)
{
	m_Center.x = x;
	m_Center.y = y;
}

glm::vec2 that::BoxCollider::GetCenterWorld() const
{
	const float rotation{ glm::radians(GetTransform()->GetWorldRotation()) };
	const float sinRot{ sinf(rotation) };
	const float cosRot{ cosf(rotation) };

	const auto& pos{ GetTransform()->GetWorldPosition() };
	return
	{
		pos.x + cosRot * m_Center.x + sinRot * m_Center.y,
		pos.y + cosRot * m_Center.y + sinRot * m_Center.x
	};
}

void that::BoxCollider::SetSize(const glm::vec2& size)
{
	SetSize(size.x, size.y);
}

void that::BoxCollider::SetSize(float x, float y)
{
	m_Size.x = x;
	m_Size.y = y;
	m_DefaultSize = false;
}

glm::vec2 that::BoxCollider::GetSizeWorld() const
{
	const float rotation{ glm::radians(GetTransform()->GetWorldRotation()) };
	const float sinRot{ sinf(rotation) };
	const float cosRot{ cosf(rotation) };

	return
	{
		abs(cosRot * m_Size.x) + abs(sinRot * m_Size.y),
		abs(cosRot * m_Size.y) + abs(sinRot * m_Size.x)
	};
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
