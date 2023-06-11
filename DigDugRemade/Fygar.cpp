#include "Fygar.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "Transform.h"

#include "FygarRoamingState.h"
#include "FygarPumpState.h"
#include "FygarRockDeathState.h"

void digdug::Fygar::Init()
{
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate.AddListener(this);

	m_Spawnpoint = GetTransform()->GetLocalPosition();
}

void digdug::Fygar::OnDestroy()
{
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate.RemoveListener(this);
}

void digdug::Fygar::Update()
{
	std::unique_ptr<EnemyState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));
}

void digdug::Fygar::Notify(const HealthComponent&)
{
	if (dynamic_cast<FygarPumpState*>(m_pState.get()) == nullptr) ChangeState(std::make_unique<FygarPumpState>(GetOwner()));
}

void digdug::Fygar::Start(const std::vector<that::GameObject*>& pPlayers, bool isPlayerControlled)
{
	m_pPlayers = pPlayers;
	m_FollowingPlayerIdx = rand() % static_cast<int>(m_pPlayers.size());
	m_IsPlayerControlled = isPlayerControlled;
	ChangeState(std::make_unique<FygarRoamingState>(GetOwner()));
}

that::GameObject* digdug::Fygar::GetPlayer()
{
	that::GameObject* pPlayer{ m_pPlayers[m_FollowingPlayerIdx] };
	while (!pPlayer->IsActive())
	{
		m_FollowingPlayerIdx = rand() % static_cast<int>(m_pPlayers.size());
	}

	return m_pPlayers[m_FollowingPlayerIdx];
}

void digdug::Fygar::RockAttack()
{
	ChangeState(std::make_unique<FygarRockDeathState>(GetOwner()));
}

void digdug::Fygar::Reset()
{
	GetTransform()->SetLocalPosition(m_Spawnpoint);
	Start(m_pPlayers, m_IsPlayerControlled);
}

void digdug::Fygar::ChangeState(std::unique_ptr<digdug::EnemyState> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
