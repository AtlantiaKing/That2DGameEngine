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
#include "Rock.h"
#include "ServiceLocator.h"
#include "AudioSystem.h"

#include "TextureManager.h"

#include "glm/vec2.hpp"

#include "RockDeathState.h"

#include "ColliderLayers.h"
#include "ScoreComponent.h"

digdug::RockFallingState::RockFallingState(that::GameObject* pRock)
	: m_pRock{ pRock }
{
}

std::unique_ptr<digdug::State> digdug::RockFallingState::Update()
{
	const float cellSize{ m_pGrid->GetCellSize() };
	const auto& position{ m_pRock->GetTransform()->GetLocalPosition() };
	const glm::ivec2 rockGridPos{ position / cellSize };

	if (m_StartPos.x == 0 && m_StartPos.y == 0) m_StartPos = rockGridPos;
	
	if (m_StartPos == rockGridPos) return nullptr;

	if (!m_pGrid->IsValidPosition(position, { 0, 1 }, true)) return std::make_unique<RockDeathState>(m_pRock);

	return nullptr;
}

void digdug::RockFallingState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Rock/Default.png") };
	m_pRock->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 1, 1);
	
	m_pRock->GetComponent<that::Rigidbody>()->SetGravityEnabled(true);

	m_pGrid = m_pRock->GetParent()->GetComponent<GridComponent>();
	m_pGrid->EnableTile(m_pRock->GetTransform()->GetLocalPosition() / m_pGrid->GetCellSize());

	m_pRock->GetComponent<that::BoxCollider>()->OnCollision.AddListener(this);
}

void digdug::RockFallingState::StateEnd()
{
	that::Rigidbody* pRb{ m_pRock->GetComponent<that::Rigidbody>() };
	pRb->SetGravityEnabled(false);
	pRb->Reset();

	m_pRock->GetComponent<that::BoxCollider>()->OnCollision.RemoveListener(this);

	if(m_EnemiesKilled > 0)
		m_pRock->GetComponent<Rock>()->GetLastPlayer()->GetComponent<ScoreComponent>()->AddScore(CalculateScore(), m_pRock->GetTransform()->GetWorldPosition());
}

void digdug::RockFallingState::Notify(const that::CollisionData& collision)
{
	if (collision.pOther->GetLayer() == ENEMY_LAYER)
	{
		if (collision.pOther->GetOwner()->GetTag() == "Pooka")
		{
			collision.pOther->GetOwner()->GetComponent<Pooka>()->RockAttack();
			that::ServiceLocator::GetAudio().Play("Sounds/RockHit.wav", 1.0f);
		}
		else
		{
			collision.pOther->GetOwner()->GetComponent<Fygar>()->RockAttack();
			that::ServiceLocator::GetAudio().Play("Sounds/RockHit.wav", 1.0f);
		}
		++m_EnemiesKilled;
	}
	else if (collision.pOther->GetLayer() == DIGDUG_LAYER)
	{
		collision.pOther->GetOwner()->GetComponent<DigDug>()->RockAttack();
		that::ServiceLocator::GetAudio().Play("Sounds/RockHit.wav", 1.0f);
	}
}

int digdug::RockFallingState::CalculateScore()
{

	switch (m_EnemiesKilled)
	{
	case 1:
		return 1000;
	case 2:
		return 2500;
	case 3:
		return 4000;
	case 4:
		return 6000;
	case 5:
		return 8000;
	case 6:
		return 10000;
	case 7:
		return 12000;
	case 8:
		return 15000;
	}

	return 0;
}
