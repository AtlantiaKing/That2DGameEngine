#pragma once

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "GridComponent.h"
#include "GridTransform.h"
#include "TextureRenderer.h"
#include "EnemyMovement.h"
#include "GridCollider.h"
#include "Player.h"
#include "LivesHUDComponent.h"
#include "TextComponent.h"
#include "Pump.h"
#include "Enemy.h"

#include "InputManager.h"
#include "ResourceManager.h"

#include "GridMoveCommand.h"
#include "ShootPumpCommand.h"

#include <sstream>
#include "ScoreHUDComponent.h"

namespace digdug
{
	void LoadGameScene(that::Scene& scene)
	{
		// Grid
		that::GameObject* pGrid{ scene.CreateGameObject("Grid") };
		auto pGridComponent{ pGrid->AddComponent<digdug::GridComponent>() };
		pGridComponent->SetCellSize(32.0f);
		pGrid->GetTransform()->SetWorldPosition(100, 40);


		// Player
		that::GameObject* pPlayer{ pGrid->CreateGameObject("Player") };
		auto pPlayerRenderer{ pPlayer->AddComponent<that::TextureRenderer>() }; 
		pPlayerRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));
		pPlayerRenderer->SetScale(2.0f);
		pPlayer->AddComponent<digdug::GridTransform>();
		pPlayer->AddComponent<digdug::GridCollider>();
		Player* pPlayerComponent{ pPlayer->AddComponent<digdug::Player>() };

		// Pump
		that::GameObject* pPump{ pPlayer->CreateGameObject("Pump") };
		auto pPumpRenderer{ pPump->AddComponent<that::TextureRenderer>() };
		pPumpRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Pump.png"));
		pPumpRenderer->SetScale(2.0f);
		pPump->AddComponent<digdug::GridCollider>();
		pPump->AddComponent<digdug::Pump>();
		pPump->GetComponent<that::Transform>()->SetLocalPosition({ pGridComponent->GetCellSize(), 0.0f });


		// Enemy
		for (int i{}; i < 5; ++i)
		{
			that::GameObject* pEnemy{ pGrid->CreateGameObject("Enemy") };
			auto pEnemyRenderer{ pEnemy->AddComponent<that::TextureRenderer>() };
			pEnemyRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
			pEnemyRenderer->SetScale(2.0f);
			pEnemy->AddComponent<digdug::EnemyMovement>();
			pEnemy->AddComponent<digdug::GridTransform>();
			pEnemy->AddComponent<digdug::GridCollider>();
			pEnemy->GetComponent<that::Transform>()->SetLocalPosition(pGridComponent->GetCellSize() * 5, pGridComponent->GetCellSize() * 2 * i);
			pEnemy->AddComponent<digdug::Enemy>();
		}


		// Lives HUD
		that::GameObject* pLivesHUD{ scene.CreateGameObject("LivesHUD") };
		pLivesHUD->GetTransform()->SetWorldPosition(0.0f, 0.0f);
		pLivesHUD->AddComponent<digdug::LivesHUDComponent>();
		that::TextComponent* pLivesText{ pLivesHUD->AddComponent<that::TextComponent>() };
		pLivesText->SetFont(that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));

		std::stringstream hudText{};
		hudText << pPlayerComponent->GetHealth() << " lifes left";
		pLivesText->SetText(hudText.str());

		pLivesHUD->AddComponent<that::TextureRenderer>();


		// Score HUD
		that::GameObject* pScoreHUD{ scene.CreateGameObject("LivesHUD") };
		pScoreHUD->GetTransform()->SetWorldPosition(0.0f, 100.0f);
		pScoreHUD->AddComponent<digdug::ScoreHUDComponent>()->SetPlayer(pPlayerComponent);
		that::TextComponent* pScoreText{ pScoreHUD->AddComponent<that::TextComponent>() };
		pScoreText->SetFont(that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));
		pScoreText->SetText("Score: 0");
		pScoreHUD->AddComponent<that::TextureRenderer>();


		// Input
		that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'q', 'z', 's' }, std::make_unique<GridMoveCommand>(pPlayer));
		that::InputManager::GetInstance().BindDigitalCommand({ ' ' }, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pPump));
	}
}