#include "DigDugDeathState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"
#include "BoxCollider.h"
#include "Transform.h"

#include "Timer.h"
#include "TextureManager.h"

#include "DigDugWalkingState.h"

digdug::DigDugDeathState::DigDugDeathState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugDeathState::HandleInput(const glm::ivec2&, bool, bool)
{
	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugDeathState::Update()
{
	if (!m_WaitingForRespawn && m_pSprite->GetTile() == 3)
	{
		m_WaitingForRespawn = true;
	}

	if (m_WaitingForRespawn)
	{
		if (m_pSprite->GetTile() == 0) m_pSprite->SetEnabled(false);

		m_WaitTime += that::Timer::GetInstance().GetElapsed();

		if (m_WaitTime > m_TimeTillRespawn)
		{
			m_pPlayer->GetComponent<GridTransform>()->SetPosition(0, 0);
			m_pPlayer->GetTransform()->SetLocalPosition(0.0f, 0.0f);
			return std::make_unique<DigDugWalkingState>(m_pPlayer);
		}
	}

	return nullptr;
}

void digdug::DigDugDeathState::StateEnter()
{
	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture("DigDug/Death.png") };
	m_pSprite = m_pPlayer->GetComponent<that::SpriteRenderer>();
	m_pSprite->SetSprite(pPlayerTexture, 4, 1, 0.2f);

	m_pPlayer->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::DigDugDeathState::StateEnd()
{
	m_pPlayer->GetComponent<that::BoxCollider>()->SetEnabled(true);
	m_pSprite->SetEnabled(true);
}
