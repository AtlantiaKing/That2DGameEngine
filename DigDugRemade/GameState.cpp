#include "GameState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "WorldTile.h"

#include "Timer.h"
#include <SceneManager.h>

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

void digdug::GameState::Update()
{
	if (m_Victory)
	{
		m_WaitTimer += that::Timer::GetInstance().GetElapsed();

		if (m_WaitTimer > m_TimeTillNextLevel)
		{
			auto& sceneManager{ that::SceneManager::GetInstance() };
			sceneManager.LoadScene(sceneManager.GetCurrentSceneIndex());
		}
	}
}

void digdug::GameState::LateUpdate()
{
	if (!m_Init)
	{
		m_Init = true;
		InitEnemies();
	}
}

void digdug::GameState::OnDestroy()
{
	for (HealthComponent* pHealth : m_pEnemies)
	{
		pHealth->OnDeath.RemoveListener(this);
	}
}

void digdug::GameState::Notify(const that::GameObject& pEnemy)
{
	m_pEnemies.erase(std::remove(begin(m_pEnemies), end(m_pEnemies), pEnemy.GetComponent<HealthComponent>()));

	if (m_pEnemies.size() == 0)
	{
		m_Victory = true;

		for (that::GameObject* pChild : GetOwner()->GetChildren())
		{
			if (pChild->HasComponent<WorldTile>()) continue;

			pChild->Destroy();
		}
	}
}
