#include "GameState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "WorldTile.h"
#include "Pooka.h"
#include "Fygar.h"

#include "GameData.h"
#include "Timer.h"
#include "SceneManager.h"
#include "GameData.h"
#include "ScoreComponent.h"
#include "InputManager.h"
#include "ServiceLocator.h"
#include "AudioSystem.h"

#include "LambdaCommand.h"

#include "StartRoundState.h"
#include "State.h"

#include "SDL_keyboard.h"

void digdug::GameState::ChangeState(std::unique_ptr<State> pState)
{
	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	if(m_pState) m_pState->StateEnter();
}

void digdug::GameState::InitEnemies()
{
	for (that::GameObject* pChild : GetOwner()->GetChildren())
	{
		HealthComponent* pHealth{ pChild->GetComponent<HealthComponent>() };
		if (pHealth && pChild->GetTag() != "DigDug")
		{
			m_pEnemies.push_back(pHealth);
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
		}
	}
}

void digdug::GameState::GoToNextRound() const
{
	auto& sceneManager{ that::SceneManager::GetInstance() };
	GameData& gameData{ GameData::GetInstance() };

	const bool isSinglePlayerScene{ sceneManager.GetCurrentSceneIndex() == gameData.GetSinglePlayerScene() };

	if (GameData::GetInstance().GetRoundNumber() == GameData::GetInstance().GetAmountRounds())
	{
		if (isSinglePlayerScene)
		{
			sceneManager.LoadScene(gameData.GetHighScoreScene());
		}
		else
		{
			sceneManager.LoadScene(gameData.GetMainMenuScene());
		}
	}
	else
	{
		sceneManager.LoadScene(sceneManager.GetCurrentSceneIndex());
	}

	GameData::GetInstance().IncrementRoundNumber();
}

std::vector<digdug::HealthComponent*>& digdug::GameState::GetEnemies()
{
	return m_pEnemies;
}

std::vector<digdug::HealthComponent*>& digdug::GameState::GetPlayers()
{
	return m_pPlayers;
}

void digdug::GameState::Clear()
{
	m_pEnemies.clear();
	m_pPlayers.clear();
}

bool digdug::GameState::IsInGame() const
{
	return dynamic_cast<StartRoundState*>(m_pState.get()) == nullptr;
}

void digdug::GameState::Init()
{
	m_pSkipLevelCommand = that::InputManager::GetInstance().BindDigitalCommand(SDLK_PAGEUP, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]() 
			{
				GoToNextRound();
			}));
}

void digdug::GameState::Update()
{
	if (m_pState == nullptr) return;

	std::unique_ptr<State> pNewState{ m_pState->Update() };
	
	if (pNewState == nullptr) return;

	ChangeState(std::move(pNewState));
}

void digdug::GameState::LateUpdate()
{
	if (!m_Init)
	{
		m_Init = true;
		InitEnemies();
		InitPlayers();
		ChangeState(std::make_unique<StartRoundState>(this));
	}
}

void digdug::GameState::OnDestroy()
{
	that::InputManager::GetInstance().Unbind(m_pSkipLevelCommand);
	ChangeState(nullptr);
}

void digdug::GameState::RestartRound()
{
	for (auto pEnemyHealth : m_pEnemies)
	{
		Pooka* pPooka{ pEnemyHealth->GetOwner()->GetComponent<Pooka>() };
		if (pPooka)
		{
			pPooka->Reset();
		}
		else
		{
			pEnemyHealth->GetOwner()->GetComponent<Fygar>()->Reset();
		}
	}

	ChangeState(std::make_unique<StartRoundState>(this));
}
