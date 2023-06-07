#include "PookaRoamingState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"

#include "PookaGhostState.h"

#include "TextureManager.h"
#include "Timer.h"

digdug::PookaRoamingState::PookaRoamingState(that::GameObject* pPooka, that::GameObject* pPlayer)
	: m_pPookaObj{ pPooka }
	, m_pPlayer{ pPlayer }
{
}


std::unique_ptr<digdug::State> digdug::PookaRoamingState::Update()
{
	m_RoamTime += that::Timer::GetInstance().GetElapsed();
	if (m_RoamTime > m_TimeUntilGhost) return std::make_unique<PookaGhostState>(m_pPookaObj, m_pPlayer);

	UpdateMovement();

	return nullptr;
}

void digdug::PookaRoamingState::StateEnter()
{
	GridTransform* pGridTransform{ m_pPookaObj->GetComponent<GridTransform>() };
	pGridTransform->SetEnabled(true);
	pGridTransform->SnapToGrid();

	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Pooka/Default.png") };
	m_pPookaObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 2, 1, 0.2f);

	m_RoamTime = 0.0f;
	m_TimeUntilGhost = static_cast<float>(rand()) / RAND_MAX * (m_MaxTimeUntilGhost - m_MinTimeUntilGhost) + m_MinTimeUntilGhost;
}

void digdug::PookaRoamingState::StateEnd()
{
	m_pPookaObj->GetComponent<GridTransform>()->SetEnabled(false);
}

void digdug::PookaRoamingState::UpdateMovement()
{
	auto pTransform{ m_pPookaObj->GetComponent<GridTransform>() };

	// Try to move
	if (pTransform->Move(m_Direction.x, m_Direction.y, true))
	{
		if (m_Direction.x)
		{
			const bool canMoveUp{ pTransform->CanMoveInDirection({ 0, 1 }) };
			const bool canMoveDown{ pTransform->CanMoveInDirection({ 0, -1 }) };

			if (canMoveDown && canMoveUp)
			{
				switch (rand() % 3)
				{
				case 0:
					break;
				case 1:
					m_Direction.x = 0;
					m_Direction.y = 1;
					break;
				case 2:
					m_Direction.x = 0;
					m_Direction.y = -1;
					break;
				}
			}
			else if (canMoveUp)
			{
				if (rand() % 2)
				{
					m_Direction.x = 0;
					m_Direction.y = 1;
				}
			}
			else if(canMoveDown)
			{
				if (rand() % 2)
				{
					m_Direction.x = 0;
					m_Direction.y = -1;
				}
			}
		}
		else
		{
			const bool canMoveRight{ pTransform->CanMoveInDirection({ 1, 0 }) };
			const bool canMoveLeft{ pTransform->CanMoveInDirection({ -1, 0 }) };

			if (canMoveLeft && canMoveRight)
			{
				switch (rand() % 3)
				{
				case 0:
					break;
				case 1:
					m_Direction.x = 1;
					m_Direction.y = 0;
					break;
				case 2:
					m_Direction.x = -1;
					m_Direction.y = 0;
					break;
				}
			}
			else if (canMoveRight)
			{
				if (rand() % 2)
				{
					m_Direction.x = 1;
					m_Direction.y = 0;
				}
			}
			else if (canMoveLeft)
			{
				if (rand() % 2)
				{
					m_Direction.x = -1;
					m_Direction.y = 0;
				}
			}
		}
		return;
	}

	// Change direction if moving fails
	if (m_Direction.y)
	{
		const bool canMoveRight{ pTransform->CanMoveInDirection({ 1, 0 }) };
		const bool canMoveLeft{ pTransform->CanMoveInDirection({ -1, 0 }) };

		m_Direction.y = 0;

		if (canMoveLeft && canMoveRight)
		{
			m_Direction.x = rand() % 2;
		}
		else
		{
			m_Direction.x = canMoveLeft ? -1 : 1;
		}
	}
	else
	{
		const bool canMoveUp{ pTransform->CanMoveInDirection({ 0, 1 }) };
		const bool canMoveDown{ pTransform->CanMoveInDirection({ 0, -1 }) };

		m_Direction.x = 0;

		if (canMoveDown && canMoveUp)
		{
			m_Direction.y = rand() % 2;
		}
		else
		{
			m_Direction.y = canMoveDown ? -1 : 1;
		}
	}
}
