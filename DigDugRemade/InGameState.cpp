#include "InGameState.h"

#include "GameObject.h"

#include "GameState.h"
#include "HealthComponent.h"

#include "ServiceLocator.h"
#include "AudioSystem.h"
#include "SceneManager.h"
#include "GameData.h"

#include "EndRoundState.h"

digdug::InGameState::InGameState(GameState* pGame)
	: m_pGame{ pGame }
{
}

std::unique_ptr<digdug::State> digdug::InGameState::Update()
{
	if (m_Victory || m_GameOver)
	{
		return std::make_unique<EndRoundState>(m_pGame, m_Victory);
	}

    return nullptr;
}

void digdug::InGameState::StateEnter()
{
	const auto& pEnemies{ m_pGame->GetEnemies() };
	const auto& pPlayers{ m_pGame->GetPlayers() };

	for (HealthComponent* pEnemy : pEnemies)
	{
		pEnemy->OnDeath.AddListener(this);
	}
	for (HealthComponent* pEnemy : pPlayers)
	{
		pEnemy->OnDeath.AddListener(this);
	}
}

void digdug::InGameState::StateEnd()
{
	const auto& pEnemies{ m_pGame->GetEnemies() };
	const auto& pPlayers{ m_pGame->GetPlayers() };

	for (HealthComponent* pHealth : pEnemies)
	{
		pHealth->OnDeath.RemoveListener(this);
	}
	for (HealthComponent* pHealth : pPlayers)
	{
		pHealth->OnDeath.RemoveListener(this);
	}
}

void digdug::InGameState::Notify(const that::GameObject& pEntity)
{
	auto& pEnemies{ m_pGame->GetEnemies() };
	auto& pPlayers{ m_pGame->GetPlayers() };

	for (HealthComponent* pHealth : pEnemies)
	{
		if (&pEntity == pHealth->GetOwner())
		{
			pHealth->OnDeath.RemoveListener(this);
			pEnemies.erase(std::remove(begin(pEnemies), end(pEnemies), pHealth));
			break;
		}
	}
	for (HealthComponent* pHealth : pPlayers)
	{
		if (&pEntity == pHealth->GetOwner())
		{
			pHealth->OnDeath.RemoveListener(this);
			pPlayers.erase(std::remove(begin(pPlayers), end(pPlayers), pHealth));
			break;
		}
	}

	if (pEnemies.size() == 0)
	{
		that::ServiceLocator::GetAudio().Play("Sounds/Victory.wav", 1.0f);
		m_Victory = true;
	}

	if (pPlayers.size() == 0)
	{
		const bool isCoOpScene{ that::SceneManager::GetInstance().GetCurrentSceneIndex() == GameData::GetInstance().GetCoOpPlayerScene() };

		if (isCoOpScene)
		{
			for (that::GameObject* pChild : m_pGame->GetOwner()->GetChildren())
			{
				if (pChild == &pEntity) continue;

				if (pChild->GetTag() == "DigDug")
				{
					if (pChild->IsActive()) return;
				}
			}
		}

		m_GameOver = true;
		that::ServiceLocator::GetAudio().Play("Sounds/GameOver.wav", 1.0f);
	}
}
