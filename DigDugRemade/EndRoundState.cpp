#include "EndRoundState.h"

#include "GameObject.h"

#include "GameState.h"
#include "WorldTile.h"
#include "GameState.h"
#include "Transform.h"
#include "UIComponent.h"

#include "Timer.h"
#include "TextureManager.h"
#include "Window.h"
#include "SceneManager.h"
#include "GameData.h"

digdug::EndRoundState::EndRoundState(GameState* pGame, bool victory)
	: m_pGame{ pGame }
	, m_Victory { victory }
{
}

std::unique_ptr<digdug::State> digdug::EndRoundState::Update()
{
	m_WaitTimer += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTimer > m_TimeTillNextLevel)
	{
		if (m_Victory)
		{
			m_pGame->GoToNextRound();
		}
		else
		{
			auto& sceneManager{ that::SceneManager::GetInstance() };
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

	return nullptr;
}

void digdug::EndRoundState::StateEnter()
{
	for (that::GameObject* pChild : m_pGame->GetOwner()->GetChildren())
	{
		if (pChild->HasComponent<WorldTile>()) continue;

		pChild->Destroy();
	}

	m_pGame->Clear();

	if (!m_Victory || GameData::GetInstance().GetRoundNumber() == GameData::GetInstance().GetAmountRounds())
	{
		const auto& screenSize{ that::Window::GetInstance().GetSize() };
		m_pGameOver = m_pGame->GetOwner()->GetScene()->CreateGameObject("GameOverHUD");
		const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture(m_Victory ? "HUD/VictoryEndGame.png" : "HUD/GameOver.png") };
		m_pGameOver->AddComponent<that::UIComponent>()->SetTexture(pTexture);
		m_pGameOver->GetTransform()->SetWorldScale(static_cast<float>(screenSize.y) / pTexture->GetSize().x);
		m_pGameOver->GetTransform()->SetLocalPosition(screenSize.x / 2.0f, screenSize.y / 2.0f);
	}
}

void digdug::EndRoundState::StateEnd()
{
}
