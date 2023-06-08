#include "RockFallingState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "Rigidbody.h"
#include "GridComponent.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Pooka.h"
#include "Fygar.h"
#include "DigDug.h"

#include "TextureManager.h"

#include "glm/vec2.hpp"

#include "RockDeathState.h"

#include "ColliderLayers.h"

digdug::RockFallingState::RockFallingState(that::GameObject* pRock, that::GameObject* pPlayer)
	: m_pRock{ pRock }
	, m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::EnemyState> digdug::RockFallingState::Update()
{
	const float cellSize{ m_pGrid->GetCellSize() };
	const auto& position{ m_pRock->GetTransform()->GetLocalPosition() };
	const glm::ivec2 rockGridPos{ position / cellSize };

	if (m_StartPos.x == 0 && m_StartPos.y == 0) m_StartPos = rockGridPos;
	
	if (m_StartPos == rockGridPos) return nullptr;

	if (!m_pGrid->IsValidPosition(position, { 0, 1 }, true)) return std::make_unique<RockDeathState>(m_pRock, m_pPlayer);

	return nullptr;
}

void digdug::RockFallingState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Rock/Default.png") };
	m_pRock->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 1, 1);
	
	m_pRock->GetComponent<that::Rigidbody>()->SetGravityEnabled(true);

	m_pGrid = m_pRock->GetParent()->GetComponent<GridComponent>();
	m_pGrid->EnableTile(m_pRock->GetTransform()->GetLocalPosition() / m_pGrid->GetCellSize());

	m_pRock->GetComponent<that::BoxCollider>()->OnHitEvent().AddListener(this);
}

void digdug::RockFallingState::StateEnd()
{
	that::Rigidbody* pRb{ m_pRock->GetComponent<that::Rigidbody>() };
	pRb->SetGravityEnabled(false);
	pRb->Reset();

	m_pRock->GetComponent<that::BoxCollider>()->OnHitEvent().RemoveListener(this);
}

void digdug::RockFallingState::Notify(const that::CollisionData& collision)
{
	if (collision.pOther->GetLayer() == ENEMY_LAYER)
	{
		if (collision.pOther->GetOwner()->GetTag() == "Pooka")
		{
			collision.pOther->GetOwner()->GetComponent<Pooka>()->RockAttack();
		}
		else
		{
			collision.pOther->GetOwner()->GetComponent<Fygar>()->RockAttack();
		}
	}
	else if (collision.pOther->GetLayer() == DIGDUG_LAYER)
	{
		collision.pOther->GetOwner()->GetComponent<DigDug>()->RockAttack();
	}
}
