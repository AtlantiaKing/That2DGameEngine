#include "DigDugPumpState.h"

#include "GameObject.h"

#include "Pump.h"
#include "SpriteRenderer.h"
#include "DigDug.h"

#include "DigDugWalkingState.h"

#include "PumpToEnemyCommand.h"

#include "Timer.h"
#include "TextureManager.h"
#include "InputManager.h"

digdug::DigDugPumpState::DigDugPumpState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugPumpState::HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold)
{
	m_PumpedThisFrame |= (pumping || pumpHold);
	m_Movement = movement;

	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugPumpState::Update()
{
	if(!m_pPump->GetOwner()->IsActive()) 
		return std::make_unique<DigDugWalkingState>(m_pPlayer);

	if(m_PumpTime < m_TimeBetweenPumps) m_PumpTime += that::Timer::GetInstance().GetElapsed();

	if (!m_PumpedThisFrame)
	{
		if (m_pPump->IsPumpedInEnemy() && (m_Movement.x || m_Movement.y)) 
			return std::make_unique<DigDugWalkingState>(m_pPlayer);

		if (m_PumpTime > m_ResetSpriteTime)
		{
			m_pSprite->SetTile(0);
		}
	}
	else
	{
		if (m_PumpTime > m_TimeBetweenPumps)
		{
			m_PumpTime -= m_TimeBetweenPumps;
			m_pPump->PumpToEnemy();

			m_pSprite->SetTile(1);
		}
		else if (m_PumpTime > m_ResetSpriteTime)
		{
			m_pSprite->SetTile(0);
		}
	}

	m_PumpedThisFrame = false;
	return nullptr;
}

void digdug::DigDugPumpState::StateEnter()
{
	that::GameObject* pPumpObj{ m_pPlayer->GetChild(0) };

	m_pPump = pPumpObj->GetComponent<Pump>();
	pPumpObj->SetActive(true);

	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture("DigDug/Pumping.png") };
	m_pSprite = m_pPlayer->GetComponent<that::SpriteRenderer>();
	m_pSprite->SetSprite(pPlayerTexture, 2, 1);

	m_pPumpEnemyCommand = that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTON, std::make_unique<PumpToEnemyCommand>(m_pPlayer->GetComponent<DigDug>()));
}

void digdug::DigDugPumpState::StateEnd()
{
	m_pPlayer->GetChild(0)->SetActive(false);

	that::InputManager::GetInstance().Unbind(m_pPumpEnemyCommand);
}
