#include "Rock.h"

#include "GameObject.h"

#include "RockStaticState.h"

void digdug::Rock::Init()
{

}

void digdug::Rock::Update()
{
	std::unique_ptr<EnemyState> pNewState{ m_pState->Update() };

	if (pNewState) ChangeState(std::move(pNewState));

	//glm::vec2 underPos{ GetTransform()->GetLocalPosition() };

	/*if (m_Static)
	{
		glm::ivec2 gridPos{ underPos };
		gridPos /= static_cast<int>(m_pGrid->GetCellSize());
		gridPos += 1;
		underPos.y = gridPos.y * m_pGrid->GetCellSize();

		if (!m_pGrid->IsOpenPosition(underPos)) return;

		m_Static = false;
		m_Rb->SetGravityEnabled(true);
	}
	else
	{
		underPos /= m_pGrid->GetCellSize();
		underPos *= m_pGrid->GetStepsPerCell();
		if (m_pGrid->IsValidPosition(underPos, {0.0f,-1.0f}, true)) return;

		m_Static = true;
		m_Rb->SetGravityEnabled(false);
		m_Rb->Reset();
	}*/
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
