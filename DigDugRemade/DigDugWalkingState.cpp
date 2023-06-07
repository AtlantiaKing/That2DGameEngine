#include "DigDugWalkingState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"	

#include "InputManager.h"
#include "TextureManager.h"

#include "DigDugPumpState.h"

digdug::DigDugWalkingState::DigDugWalkingState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
	, m_pTransform{ m_pPlayer->GetComponent<GridTransform>() }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugWalkingState::HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold)
{
	if(movement.x || movement.y)
		m_IsWalking = m_pTransform->Move(movement.x, movement.y);

	if (pumping && !pumpHold) return std::make_unique<DigDugPumpState>(m_pPlayer);

	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugWalkingState::Update()
{
	if (m_PrevWalking != m_IsWalking)
	{
		m_pPlayer->GetComponent<that::SpriteRenderer>()->SetTimePerTile(m_IsWalking ? m_SpriteSpeed : FLT_MAX);
	}

	m_PrevWalking = m_IsWalking;
	m_IsWalking = false;
	return nullptr;
}

void digdug::DigDugWalkingState::StateEnter()
{
	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture("DigDug/Walking.png") };
	m_pPlayer->GetComponent<that::SpriteRenderer>()->SetSprite(pPlayerTexture, 2, 1);
}

void digdug::DigDugWalkingState::StateEnd()
{
}
