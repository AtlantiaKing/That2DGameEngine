#include "Rock.h"

#include "GameObject.h"

#include "GridComponent.h"
#include "Transform.h"

#include "RockStaticState.h"

void digdug::Rock::Init()
{
	GridComponent* pGrid{ GetOwner()->GetParent()->GetComponent<GridComponent>() };
	
	pGrid->DisableTile(GetTransform()->GetLocalPosition() / pGrid->GetCellSize());
}

void digdug::Rock::Update()
{
	std::unique_ptr<EnemyState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));
}

void digdug::Rock::Start(that::GameObject* pPlayer)
{
	ChangeState(std::make_unique<RockStaticState>(GetOwner(), pPlayer));
}

void digdug::Rock::ChangeState(std::unique_ptr<digdug::EnemyState> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
