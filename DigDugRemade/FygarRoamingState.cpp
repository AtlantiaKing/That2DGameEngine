#include "FygarRoamingState.h"


#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"
#include "Fygar.h"

#include "FygarGhostState.h"
#include "FygarAttackState.h"

#include "TextureManager.h"
#include "Timer.h"

#include "FygarRoamingAI.h"
#include "FygarRoamingPlayer.h"

digdug::FygarRoamingState::FygarRoamingState(that::GameObject* pFygar)
	: m_pFygarObj{ pFygar }
{
}

std::unique_ptr<digdug::EnemyState> digdug::FygarRoamingState::Update()
{
	return m_pLogic->Update();
}

void digdug::FygarRoamingState::StateEnter()
{
	GridTransform* pGridTransform{ m_pFygarObj->GetComponent<GridTransform>() };
	pGridTransform->SetEnabled(true);
	pGridTransform->SnapToGrid();

	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/Default.png") };
	m_pFygarObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 2, 1, 0.2f);

	if (m_pFygarObj->GetComponent<Fygar>()->IsPlayerControlled())
	{
		m_pLogic = std::make_unique<FygarRoamingPlayer>(m_pFygarObj);
	}
	else
	{
		m_pLogic = std::make_unique<FygarRoamingAI>(m_pFygarObj);
	}
}

void digdug::FygarRoamingState::StateEnd()
{
	GridTransform* pGridTransform{ m_pFygarObj->GetComponent<GridTransform>() };
	pGridTransform->SetEnabled(false);
}
