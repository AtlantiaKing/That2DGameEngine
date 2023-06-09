#include "GameScene.h"

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

// Engine includes
#include "Window.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "GameData.h"

// Library includes
#include <iostream>
#include "glm/vec2.hpp"

void digdug::GameScene::Load(that::Scene& scene)
{
	// Grid
	that::GameObject* pGrid{ scene.CreateGameObject("Grid") };
	pGrid->AddComponent<LevelLoader>()->SetLevel("Levels/DemoLevel.png", 1);
	pGrid->AddComponent<GameState>();

	constexpr float referenceSize{ 240.0f };
	const auto& windowSize{ that::Window::GetInstance().GetSize() };
	const float scale{ windowSize.x / referenceSize };
	that::GameObject* pHUD{ scene.CreateGameObject("HUD") };
	pHUD->GetTransform()->SetWorldScale(scale * pGrid->GetTransform()->GetWorldScale().x);
	{
		that::GameObject* pHighScoreHeader{ pHUD->CreateGameObject("HighScoreHeader") };
		const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("HUD/HighScore.png") };
		const auto& pTextureSize{ pTexture->GetSize() };
		const auto& pRenderer{ pHighScoreHeader->AddComponent<that::TextureRenderer>() };
		pRenderer->SetTexture(pTexture);
		pRenderer->SetPivot({ 1.0f, 0.5f });
		pHighScoreHeader->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
		pHighScoreHeader->GetTransform()->Translate(0.0f, 2.0f + pTextureSize.y / 2.0f);


		that::GameObject* pScore{ pHUD->CreateGameObject("HighScore") };
		pScore->AddComponent<that::TextureRenderer>()->SetPivot({ 1.0f, 0.5f });
		that::TextComponent* pScoreText{ pScore->AddComponent<that::TextComponent>() };
		pScoreText->SetFont(that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8));
		pScoreText->SetText("0");
		pScore->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
		pScore->GetTransform()->Translate(0.0f, pHighScoreHeader->GetTransform()->GetLocalPosition().y + pTextureSize.y);
		pScore->AddComponent<HighScoreHUDComponent>()->SearchPlayer(pGrid);
	}
	{
		that::GameObject* pScoreHeader{ pHUD->CreateGameObject("ScoreHeader") };
		const auto& pHeaderTexture{ that::TextureManager::GetInstance().LoadTexture("HUD/Score.png") };
		const auto& pTextureSize{ pHeaderTexture->GetSize() };
		const auto& pHeaderRenderer{ pScoreHeader->AddComponent<that::TextureRenderer>() };
		pHeaderRenderer->SetTexture(pHeaderTexture);
		pHeaderRenderer->SetPivot({ 1.0f, 0.5f });
		pScoreHeader->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
		pScoreHeader->GetTransform()->Translate(0.0f, 37.0f + pTextureSize.y / 2.0f);


		that::GameObject* pScore{ pHUD->CreateGameObject("Score") };
		pScore->AddComponent<that::TextureRenderer>()->SetPivot({ 1.0f, 0.5f });
		that::TextComponent* pScoreText{ pScore->AddComponent<that::TextComponent>() };
		pScoreText->SetFont(that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8));
		pScoreText->SetText("0");
		pScore->GetTransform()->SetWorldPosition(static_cast<float>(windowSize.x), 0.0f);
		pScore->GetTransform()->Translate(0.0f , pScoreHeader->GetTransform()->GetLocalPosition().y + pTextureSize.y);
		pScore->AddComponent<ScoreHUDComponent>()->SearchPlayer(pGrid);
	}
	{
		that::GameObject* pHealth{ pHUD->CreateGameObject("Health") };
		pHealth->GetTransform()->SetLocalPosition(192.0f, 122.0f);
		pHealth->AddComponent<LivesHUDComponent>()->SearchPlayer(pGrid);
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