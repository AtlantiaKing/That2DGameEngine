#include "DisableOnGroundHit.h"

#include "GameObject.h"

#include "TextureMask.h"
#include "TextureComponent.h"
#include "Transform.h"
#include "GridComponent.h"

#include "Renderer.h"

#include "glm/vec2.hpp"

#include <iostream>

void digdug::DisableOnGroundHit::Init()
{
	that::GameObject* pParent{ GetOwner()->GetParent() };

	m_pMask = pParent->GetComponent<that::TextureMask>();
	m_pTexture = pParent->GetComponent<that::TextureComponent>();

	// Parent = pump/breath, next parent is player/enemy, next parent is the grid
	m_pGrid = pParent->GetParent()->GetParent()->GetComponent<GridComponent>();
}

void digdug::DisableOnGroundHit::Update()
{
	// Get the position on the far side of the mask
	const int textureSize{ m_pTexture->GetTextureSize().x };
	const float maskedTextureSize{ m_pMask->GetMask().x * textureSize };

	GetTransform()->SetLocalPosition(-textureSize / 2.0f + maskedTextureSize, 0.0f);

	const float direction{ GetOwner()->GetParent()->GetParent()->GetTransform()->GetLocalScale().y };
	const float rotation{ GetOwner()->GetParent()->GetParent()->GetTransform()->GetLocalRotation() };
	
	glm::vec2 posRelativeToEntity{ (GetOwner()->GetParent()->GetTransform()->GetLocalPosition() + GetTransform()->GetLocalPosition()) };

	if (abs(rotation - 90.0f) < FLT_EPSILON || abs(rotation + 90) < FLT_EPSILON)
	{
		const float x{ posRelativeToEntity.x };
		posRelativeToEntity.x = posRelativeToEntity.y;
		posRelativeToEntity.y = -x;
	}

	glm::vec2 gridPosition{ GetOwner()->GetParent()->GetParent()->GetTransform()->GetLocalPosition() + posRelativeToEntity * direction };

	if (!m_pGrid->IsValidPixel(gridPosition)) GetOwner()->GetParent()->SetActive(false);
}
