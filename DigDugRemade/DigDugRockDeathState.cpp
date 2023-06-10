#include "DigDugRockDeathState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"
#include "Transform.h"
#include "HealthComponent.h"
#include "BoxCollider.h"
#include "DigDug.h"
#include "GridComponent.h"

#include "TextureManager.h"
#include "Timer.h"

#include "DigDugWalkingState.h"
#include "DigDugNoLivesState.h"

digdug::DigDugRockDeathState::DigDugRockDeathState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugRockDeathState::HandleInput(const glm::ivec2&, bool, bool)
{
	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugRockDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillRespawn)
	{
		if (m_pPlayer->GetComponent<HealthComponent>()->GetHealth() == 0) return std::make_unique<DigDugNoLivesState>(m_pPlayer);

		GridComponent* pGrid{ m_pPlayer->GetParent()->GetComponent<GridComponent>() };
		const float cellSize{ pGrid->GetCellSize() };
		const glm::ivec2 gridSpawnPos{ m_pPlayer->GetComponent<DigDug>()->GetSpawnPoint() / cellSize };

		m_pPlayer->GetComponent<GridTransform>()->SetPosition(gridSpawnPos.x, gridSpawnPos.y);
		m_pPlayer->GetTransform()->SetLocalPosition(m_pPlayer->GetComponent<DigDug>()->GetSpawnPoint());
		return std::make_unique<DigDugWalkingState>(m_pPlayer);
	}

	return nullptr;
}

void digdug::DigDugRockDeathState::StateEnter()
{
	std::stringstream texturePath{};
	texturePath << "DigDug" << m_pPlayer->GetComponent<DigDug>()->GetPlayerIndex() << "/DeathByRock.png";

	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture(texturePath.str()) };
	m_pPlayer->GetComponent<that::SpriteRenderer>()->SetSprite(pPlayerTexture, 1, 1);
	m_pPlayer->GetComponent<HealthComponent>()->Hit();

	m_pPlayer->GetComponent<that::BoxCollider>()->SetEnabled(false);

	m_pPlayer->GetTransform()->SetLocalRotation(0);
	m_pPlayer->GetTransform()->SetLocalScale(1.0f);
}

void digdug::DigDugRockDeathState::StateEnd()
{
	m_pPlayer->GetComponent<that::BoxCollider>()->SetEnabled(true);
}
