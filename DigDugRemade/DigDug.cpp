#include "DigDug.h"

#include "GameObject.h"

#include "BoxCollider.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "Transform.h"

#include "DigDugWalkingState.h"
#include "DigDugDeathState.h"
#include "DigDugRockDeathState.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "GameData.h"

#include "ColliderLayers.h"

void digdug::DigDug::Init()
{
	m_SpawnPoint = GetTransform()->GetLocalPosition();

	ChangeState(std::make_unique<DigDugWalkingState>(GetOwner()));

	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().AddListener(this);

	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate.AddListener(this);
}

void digdug::DigDug::OnDestroy()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().RemoveListener(this);
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate.RemoveListener(this);
	that::InputManager::GetInstance().Clear();

	GameData::GetInstance().SetHealth(m_PlayerIndex, GetOwner()->GetComponent<HealthComponent>()->GetHealth());
}

void digdug::DigDug::Update()
{
	ChangeState(m_pState->Update());
}

void digdug::DigDug::Notify(const that::CollisionData& collision)
{
	if (collision.pOther->GetLayer() == ENEMY_LAYER)
	{
		GetOwner()->GetComponent<HealthComponent>()->Hit();
	}
}

void digdug::DigDug::Notify(const HealthComponent&)
{
	if (dynamic_cast<DigDugRockDeathState*>(m_pState.get())) return;

	ChangeState(std::make_unique<DigDugDeathState>(GetOwner()));
}

void digdug::DigDug::Move(const glm::vec2& movementInput)
{
	ChangeState(m_pState->HandleInput(movementInput, false, false));
}

void digdug::DigDug::Pump(bool hold)
{
	ChangeState(m_pState->HandleInput(m_DefaultMovement, true, hold));
}

void digdug::DigDug::RockAttack()
{
	ChangeState(std::make_unique<DigDugRockDeathState>(GetOwner()));
}

void digdug::DigDug::DisablePump()
{
	ChangeState(std::make_unique<DigDugWalkingState>(GetOwner()));
}

void digdug::DigDug::SetPlayerIndex(int index)
{
	m_PlayerIndex = index;
}

int digdug::DigDug::GetPlayerIndex() const
{
	return m_PlayerIndex;
}

void digdug::DigDug::SetAudio(DigDugAudio* pAudio)
{
	m_pAudio = pAudio;
}

digdug::DigDugAudio* digdug::DigDug::GetAudio() const
{
	return m_pAudio;
}

void digdug::DigDug::ChangeState(std::unique_ptr<DigDugState> pState)
{
	if (!pState) return;

	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
