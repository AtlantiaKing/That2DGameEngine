#include "CoOpGameScene.h"

// Objects
#include "Scene.h"
#include "GameObject.h"

// Components
#include "Transform.h"
#include "LevelLoader.h"
#include "GridComponent.h"
#include "TextComponent.h"
#include "TextureRenderer.h"
#include "ScoreHUDComponent.h"
#include "LivesHUDComponent.h"
#include "HighScoreHUDComponent.h"
#include "GameState.h"
#include "CameraComponent.h"

// Engine includes
#include "Window.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "GameData.h"

// Library includes
#include "glm/vec2.hpp"

// Std includes
#include <iostream>
#include <sstream>

void digdug::CoOpGameScene::Load(that::Scene& scene)
{
	const auto& windowSize{ that::Window::GetInstance().GetSize() };

	// Camera
	that::GameObject* pCamera{ scene.CreateGameObject("Camera") };
	pCamera->AddComponent<that::CameraComponent>()->SetAsMainCamera();
	pCamera->GetTransform()->SetWorldPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	constexpr int nrPlayers{ 2 };

	// Grid
	that::GameObject* pGrid{ scene.CreateGameObject("Grid") };

	std::stringstream levelFile{};
	levelFile << "Levels/Level" << GameData::GetInstance().GetRoundNumber() << "_2.png";

	pGrid->AddComponent<LevelLoader>()->SetLevel(levelFile.str(), 2, false);
	pGrid->AddComponent<GameState>();

	constexpr float referenceSize{ 240.0f };
	const float scale{ windowSize.x / referenceSize };
	that::GameObject* pHUD{ scene.CreateGameObject("HUD") };
	pHUD->GetTransform()->SetWorldScale(scale * pGrid->GetTransform()->GetWorldScale().x);

	constexpr float distanceBetweenPlayerHuds{ 60.0 };

	for (int i{}; i < nrPlayers; ++i)
	{
		{
			std::stringstream scoreFileName{};
			scoreFileName << "HUD/Score" << (i+1) << ".png";

			that::GameObject* pScoreHeader{ pHUD->CreateGameObject("ScoreHeader") };
			const auto& pHeaderTexture{ that::TextureManager::GetInstance().LoadTexture(scoreFileName.str()) };
			const auto& pTextureSize{ pHeaderTexture->GetSize() };
			const auto& pHeaderRenderer{ pScoreHeader->AddComponent<that::TextureRenderer>() };
			pHeaderRenderer->SetTexture(pHeaderTexture);
			pHeaderRenderer->SetPivot({ 1.0f, 0.5f });
			pScoreHeader->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
			pScoreHeader->GetTransform()->Translate(0.0f, 37.0f + distanceBetweenPlayerHuds * i + pTextureSize.y / 2.0f);


			that::GameObject* pScore{ pHUD->CreateGameObject("Score") };
			pScore->AddComponent<that::TextureRenderer>()->SetPivot({ 1.0f, 0.5f });
			that::TextComponent* pScoreText{ pScore->AddComponent<that::TextComponent>() };
			pScoreText->SetFont(that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8));
			pScoreText->SetText("0");
			pScore->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
			pScore->GetTransform()->Translate(0.0f, pScoreHeader->GetTransform()->GetLocalPosition().y + pTextureSize.y);
			pScore->AddComponent<ScoreHUDComponent>()->SearchPlayer(pGrid, i);
		}
		{
			that::GameObject* pHealth{ pHUD->CreateGameObject("Health") };
			pHealth->GetTransform()->SetLocalPosition(192.0f, 60.0f + distanceBetweenPlayerHuds * i);
			pHealth->AddComponent<LivesHUDComponent>()->SearchPlayer(pGrid, i);
		}
		{
			that::GameObject* pRoundHeader{ pHUD->CreateGameObject("RoundHeader") };
			const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("HUD/Round.png") };
			const auto& pTextureSize{ pTexture->GetSize() };
			const auto& pRenderer{ pRoundHeader->AddComponent<that::TextureRenderer>() };
			pRenderer->SetTexture(pTexture);
			pRenderer->SetPivot({ 1.0f, 0.5f });
			pRoundHeader->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
			pRoundHeader->GetTransform()->Translate(0.0f, 163.0f + pTextureSize.y / 2.0f);


			that::GameObject* pRound{ pHUD->CreateGameObject("RoundNumber") };
			pRound->AddComponent<that::TextureRenderer>()->SetPivot({ 1.0f, 0.5f });
			that::TextComponent* pScoreText{ pRound->AddComponent<that::TextComponent>() };
			pScoreText->SetFont(that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8));
			pScoreText->SetText(std::to_string(GameData::GetInstance().GetRoundNumber()));
			pRound->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
			pRound->GetTransform()->Translate(0.0f, pRoundHeader->GetTransform()->GetLocalPosition().y + pTextureSize.y);
		}
	}
}
