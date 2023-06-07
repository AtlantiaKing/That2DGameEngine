#include "Pooka.h"

#include "GameObject.h"

#include "HealthComponent.h"

#include "PookaRoamingState.h"
#include "PookaPumpState.h"

void digdug::Pooka::Init()
{
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate().AddListener(this);
}

void digdug::Pooka::Update()
{
	std::unique_ptr<EnemyState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));
}

void digdug::Pooka::Notify(const HealthComponent&)
{
	if (dynamic_cast<PookaPumpState*>(m_pState.get()) == nullptr) ChangeState(std::make_unique<PookaPumpState>(GetOwner(), m_pPlayer));
}

void digdug::Pooka::Start(that::GameObject* pPlayer)
{
	m_pPlayer = pPlayer;
	ChangeState(std::make_unique<PookaRoamingState>(GetOwner(), pPlayer));
}

void digdug::Pooka::ChangeState(std::unique_ptr<digdug::EnemyState> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
