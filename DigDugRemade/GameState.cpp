#include "GameState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "WorldTile.h"
#include "GameData.h"

#include "Timer.h"
#include <SceneManager.h>
#include "GameData.h"
#include "ScoreComponent.h"

void digdug::GameState::InitEnemies()
{
	for (that::GameObject* pChild : GetOwner()->GetChildren())
	{
		HealthComponent* pHealth{ pChild->GetComponent<HealthComponent>() };
		if (pHealth && pChild->GetTag() != "DigDug")
		{
			m_pEnemies.push_back(pHealth);
			pHealth->OnDeath.AddListener(this);
		}
	}
}

void digdug::GameState::InitPlayers()
{
	for (that::GameObject* pChild : GetOwner()->GetChildren())
	{
		HealthComponent* pHealth{ pChild->GetComponent<HealthComponent>() };
		if (pHealth && pChild->GetTag() == "DigDug")
		{
			m_pPlayers.push_back(pHealth);
			pHealth->OnDeath.AddListener(this);
		}
	}
}

void digdug::GameState::Update()
{
	if (m_Victory || m_GameOver)
	{
		m_WaitTimer += that::Timer::GetInstance().GetElapsed();

		if (m_WaitTimer > m_TimeTillNextLevel)
		{
			auto& sceneManager{ that::SceneManager::GetInstance() };
			
			if (m_Victory)
			{
				sceneManager.LoadScene(sceneManager.GetCurrentSceneIndex());
			}
			else
			{
				GameData& gameData{ GameData::GetInstance() };

				const bool isSinglePlayerScene{ sceneManager.GetCurrentSceneIndex() == gameData.GetSinglePlayerScene() };

				if (isSinglePlayerScene && gameData.TryNewHighScore(gameData.GetCurrentScores(0)))
				{
					sceneManager.LoadScene(gameData.GetHighScoreScene());
				}
				else
				{
					sceneManager.LoadScene(gameData.GetMainMenuScene());
				}
			}
		}
	}
}

void digdug::GameState::LateUpdate()
{
	if (!m_Init)
	{
		m_Init = true;
		InitEnemies();
		InitPlayers();
	}
}

void digdug::GameState::OnDestroy()
{
	for (HealthComponent* pHealth : m_pEnemies)
	{
		pHealth->OnDeath.RemoveListener(this);
	}
	for (HealthComponent* pHealth : m_pPlayers)
	{
		pHealth->OnDeath.RemoveListener(this);
	}
	GameData::GetInstance().IncrementRoundNumber();
}

void digdug::GameState::Notify(const that::GameObject& pEntity)
{
	if (m_Victory || m_GameOver) return;

	for (HealthComponent* pHealth : m_pEnemies)
	{
		if (pEntity.GetComponent<HealthComponent>() == pHealth)
		{
			pHealth->OnDeath.RemoveListener(this);
			m_pEnemies.erase(std::remove(begin(m_pEnemies), end(m_pEnemies), pHealth));
			break;
		}
	}
	for (HealthComponent* pHealth : m_pPlayers)
	{
		if (pEntity.GetComponent<HealthComponent>() == pHealth)
		{
			pHealth->OnDeath.RemoveListener(this);
			m_pPlayers.erase(std::remove(begin(m_pPlayers), end(m_pPlayers), pHealth));
			break;
		}
	}

	if (m_pEnemies.size() == 0)
	{
		m_Victory = true;
	}

	if (m_pPlayers.size() == 0)
	{
		const bool isCoOpScene{ that::SceneManager::GetInstance().GetCurrentSceneIndex() == GameData::GetInstance().GetCoOpPlayerScene()};

		if (isCoOpScene)
		{
			for (that::GameObject* pChild : GetOwner()->GetChildren())
			{
				if (pChild == &pEntity) continue;

				if (pChild->GetTag() == "DigDug")
				{
					if (pChild->IsActive()) return;
				}
			}
		}

		m_GameOver = true;
	}

	if (m_Victory || m_GameOver)
	{
		for (that::GameObject* pChild : GetOwner()->GetChildren())
		{
			if (pChild->HasComponent<WorldTile>()) continue;

			pChild->Destroy();
		}

		m_pEnemies.clear();
		m_pPlayers.clear();
	}
}
