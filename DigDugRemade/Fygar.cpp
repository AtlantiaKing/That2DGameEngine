#include "Fygar.h"

#include "GameObject.h"

#include "HealthComponent.h"

#include "FygarRoamingState.h"
#include "FygarPumpState.h"

void digdug::Fygar::Init()
{
	GetOwner()->GetComponent<HealthComponent>()->OnHealthUpdate().AddListener(this);
}

void digdug::Fygar::Update()
{
	std::unique_ptr<FygarState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));
}

void digdug::Fygar::Notify(const HealthComponent&)
{
	if (dynamic_cast<FygarPumpState*>(m_pState.get()) == nullptr) ChangeState(std::make_unique<FygarPumpState>(GetOwner(), m_pPlayer));
}

void digdug::Fygar::Start(that::GameObject* pPlayer)
{
	m_pPlayer = pPlayer;
	ChangeState(std::make_unique<FygarRoamingState>(GetOwner(), pPlayer));
}

void digdug::Fygar::ChangeState(std::unique_ptr<digdug::FygarState> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}