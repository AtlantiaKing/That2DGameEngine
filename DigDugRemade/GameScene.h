#pragma once

// Objects
#include "Scene.h"
#include "GameObject.h"

// Components
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
#include "ScoreHUDComponent.h"

// Managers
#include "InputManager.h"
#include "ResourceManager.h"

// Commands
#include "GridMoveCommand.h"
#include "ShootPumpCommand.h"

// STD includes
#include <sstream>
#include <iostream>
#include "EnemyBehaviour.h"
#include "WorldTile.h"

namespace digdug
{
	that::GameObject* CreatePlayerAndHUD(that::Scene& scene, that::GameObject* pGrid, int idx)
	{
		GridComponent* pGridComponent{ pGrid->GetComponent<GridComponent>() };

		// Player
		that::GameObject* pPlayer{ pGrid->CreateGameObject("Player") };
		auto pPlayerRenderer{ pPlayer->AddComponent<that::TextureRenderer>() };
		pPlayerRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));
		pPlayer->AddComponent<GridTransform>();
		pPlayer->AddComponent<GridCollider>();

		pPlayer->AddComponent<Player>();

		constexpr int defaultHealth{ 3 };
		HealthComponent* pPlayerHealth{ pPlayer->AddComponent<HealthComponent>() };
		pPlayerHealth->SetMaxHealth(defaultHealth);
		pPlayerHealth->SetDestroyOnDeath(false);

		pPlayer->AddComponent<digdug::ScoreComponent>();

		// Pump
		that::GameObject* pPump{ pPlayer->CreateGameObject("Pump") };
		auto pPumpRenderer{ pPump->AddComponent<that::TextureRenderer>() };
		pPumpRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Pump.png"));
		pPump->AddComponent<GridCollider>();
		pPump->AddComponent<Pump>();
		// Move the pump one cell to the right
		pPump->GetComponent<that::Transform>()->Translate(pGridComponent->GetCellSize(), 0.0f);


		// Input
		if (idx == 0)
		{
			that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'a', 'w', 's' }, std::make_unique<GridMoveCommand>(pPlayer));
			that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pPump));
		}
		else
		{
			that::InputManager::GetInstance().BindDigital2DAxisCommand(
				0, 
				{ 
					that::InputManager::GamepadButton::DPAD_RIGHT, 
					that::InputManager::GamepadButton::DPAD_LEFT, 
					that::InputManager::GamepadButton::DPAD_UP, 
					that::InputManager::GamepadButton::DPAD_DOWN },
				std::make_unique<GridMoveCommand>(pPlayer)
			);
			that::InputManager::GetInstance().BindDigitalCommand(
				0, 
				that::InputManager::GamepadButton::A, 
				that::InputManager::InputType::ONBUTTONDOWN, 
				std::make_unique<ShootPumpCommand>(pPump)
			);
		}


		// Lives HUD
		that::GameObject* pLivesHUD{ scene.CreateGameObject("LivesHUD") };
		pLivesHUD->GetTransform()->SetWorldPosition(idx ? 500.0f : 0.0f, 0.0f);
		pLivesHUD->AddComponent<digdug::LivesHUDComponent>()->Display(pPlayer);

		that::TextComponent* pLivesText{ pLivesHUD->AddComponent<that::TextComponent>() };
		pLivesText->SetFont(that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));
		std::stringstream hudText{};
		hudText << pPlayerHealth->GetHealth() << " lifes left";
		pLivesText->SetText(hudText.str());

		pLivesHUD->AddComponent<that::TextureRenderer>();


		// Score HUD
		that::GameObject* pScoreHUD{ scene.CreateGameObject("LivesHUD") };
		pScoreHUD->GetTransform()->SetWorldPosition(idx ? 500.0f : 0.0f, 100.0f);
		pScoreHUD->AddComponent<digdug::ScoreHUDComponent>()->Display(pPlayer);

		that::TextComponent* pScoreText{ pScoreHUD->AddComponent<that::TextComponent>() };
		pScoreText->SetFont(that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));
		pScoreText->SetText("Score: 0");

		pScoreHUD->AddComponent<that::TextureRenderer>();

		return pPlayer;
	}

	void PrintControls()
	{
		std::cout << "\n";
		std::cout << "Controls for DigDug Remade\n";
		std::cout << "\n";
		std::cout << "Player 1:\n";
		std::cout << "Move: WASD\n";
		std::cout << "Shoot pump: Space bar\n";
		std::cout << "\n";
		std::cout << "Player 2:\n";
		std::cout << "Move: D-Pad\n";
		std::cout << "Shoot pump: South Button (A on XBox)\n";
		std::cout << "\n";
	}

	void LoadGameScene(that::Scene& scene)
	{
		// Grid
		that::GameObject* pGrid{ scene.CreateGameObject("Grid") };
		auto pGridComponent{ pGrid->AddComponent<digdug::GridComponent>() };
		constexpr float gridCellSize{ 32.0f };
		pGridComponent->SetCellSize(gridCellSize);
		pGrid->GetTransform()->SetWorldPosition(100, 40);

		std::vector<that::GameObject*> pTileObjects{};
		
		for (int x{}; x < pGridComponent->GetSize(); ++x)
		{
			for (int y{}; y < pGridComponent->GetSize(); ++y)
			{
				that::GameObject* pTile{ pGrid->CreateGameObject("WorldTile") };
				pTile->AddComponent<digdug::GridTransform>();
				pTile->GetComponent<that::Transform>()->SetLocalPosition(x * gridCellSize, y * gridCellSize);
				auto pTexture{ pTile->AddComponent<that::TextureRenderer>() };
				pTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("WorldTile.png"));
				pTexture->SetScale(2.0f);

				pTileObjects.push_back(pTile);
			}
		}

		that::GameObject* pPlayer0{ CreatePlayerAndHUD(scene, pGrid, 0) };
		CreatePlayerAndHUD(scene, pGrid, 1);

		for (auto pTile : pTileObjects)
		{
			pTile->AddComponent<digdug::WorldTile>()->BindPlayer(pPlayer0->GetComponent<digdug::GridTransform>());
		}

		// Enemy
		for (int i{}; i < 10; ++i)
		{
			that::GameObject* pEnemy{ pGrid->CreateGameObject("Enemy") };
			auto pEnemyRenderer{ pEnemy->AddComponent<that::TextureRenderer>() };
			pEnemyRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
			pEnemy->AddComponent<digdug::EnemyMovement>();
			pEnemy->AddComponent<digdug::GridTransform>();
			pEnemy->AddComponent<digdug::GridCollider>();
			pEnemy->GetComponent<that::Transform>()->SetLocalPosition(pGridComponent->GetCellSize() * (i % 2 * 8), pGridComponent->GetCellSize() * (i+1));
			pEnemy->AddComponent<digdug::HealthComponent>()->SetMaxHealth(4);
			pEnemy->AddComponent<digdug::EnemyBehaviour>();
		}

		PrintControls();
	}
}