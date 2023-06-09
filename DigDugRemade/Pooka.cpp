#include "Pooka.h"

#include "GameObject.h"

#include "HealthComponent.h"

#include "PookaRoamingState.h"
#include "PookaPumpState.h"
#include "PookaRockDeathState.h"

void digdug::Pooka::Init()
{
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate.AddListener(this);
}

void digdug::Pooka::OnDestroy()
{
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate.RemoveListener(this);
}

void digdug::Pooka::Update()
{
	std::unique_ptr<EnemyState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));
}

void digdug::Pooka::Notify(const HealthComponent&)
{
	if (dynamic_cast<PookaPumpState*>(m_pState.get()) == nullptr && dynamic_cast<PookaRockDeathState*>(m_pState.get()) == nullptr) ChangeState(std::make_unique<PookaPumpState>(GetOwner()));
}

void digdug::Pooka::Start(const std::vector<that::GameObject*>& pPlayers)
{
	m_pPlayers = pPlayers;
	m_FollowingPlayerIdx = rand() % static_cast<int>(m_pPlayers.size());
	ChangeState(std::make_unique<PookaRoamingState>(GetOwner()));
}

that::GameObject* digdug::Pooka::GetPlayer()
{
	that::GameObject* pPlayer{ m_pPlayers[m_FollowingPlayerIdx] };
	while(!pPlayer->IsActive())
	{
		m_FollowingPlayerIdx = rand() % static_cast<int>(m_pPlayers.size());
		pPlayer = m_pPlayers[m_FollowingPlayerIdx];
	}

	return m_pPlayers[m_FollowingPlayerIdx];
}

void digdug::Pooka::RockAttack()
{
	ChangeState(std::make_unique<PookaRockDeathState>(GetOwner()));
}

void digdug::Pooka::ChangeState(std::unique_ptr<digdug::EnemyState> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
