#include "GameScene.h"

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
#include "TextureMask.h"
#include "AudioSource.h"
#include "BoxCollider.h"
#include "Rigidbody.cpp"

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

void digdug::GameScene::Load(that::Scene& scene)
{
	// Grid
	that::GameObject* pGrid{ scene.CreateGameObject("Grid") };
	auto pGridComponent{ pGrid->AddComponent<digdug::GridComponent>() };
	constexpr float gridCellSize{ 32.0f };
	pGridComponent->SetCellSize(gridCellSize);
	pGrid->GetTransform()->SetWorldPosition(100, 40);

	const int gridSize{ pGridComponent->GetSize() };
	const float cellSize{ pGridComponent->GetCellSize() };
	for (int x{}; x < gridSize; ++x)
	{
		for (int y{}; y < gridSize; ++y)
		{
			if (y == gridSize / 2 && x >= gridSize / 2 - 1 && x <= gridSize / 2 + 1) continue;

			that::GameObject* pTile{ pGrid->CreateGameObject("WorldTile") };

			pTile->GetComponent<that::Transform>()->SetLocalPosition(x * cellSize, y * cellSize);

			auto pTexture{ pTile->AddComponent<that::TextureRenderer>() };
			pTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("WorldTile.png"));
			pTexture->SetScale(2.0f);

			pGridComponent->SetTile(x, y, pTile->AddComponent<WorldTile>());
		}
	}

	that::GameObject* pPlayer{ CreatePlayer(pGrid) };

	pGridComponent->BindPlayer(pPlayer->GetComponent<GridTransform>());

	// Enemy
	that::GameObject* pEnemy{ pGrid->CreateGameObject("Enemy") };
	auto pEnemyRenderer{ pEnemy->AddComponent<that::TextureRenderer>() };
	pEnemyRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
	pEnemy->AddComponent<EnemyMovement>();
	pEnemy->AddComponent<GridTransform>();
	pEnemy->AddComponent<that::BoxCollider>();
	pEnemy->GetComponent<that::Transform>()->SetLocalPosition(pGridComponent->GetCellSize() * 6, pGridComponent->GetCellSize() * 6);
	pEnemy->AddComponent<HealthComponent>()->SetMaxHealth(4);
	pEnemy->AddComponent<EnemyBehaviour>();

	PrintControls();
}

that::GameObject* digdug::GameScene::CreatePlayer(that::GameObject* pGrid)
{
	GridComponent* pGridComponent{ pGrid->GetComponent<GridComponent>() };

	// Player
	that::GameObject* pPlayer{ pGrid->CreateGameObject("Player") };
	pPlayer->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));
	pPlayer->AddComponent<GridTransform>();
	pPlayer->AddComponent<that::BoxCollider>();
	pPlayer->AddComponent<Player>();
	that::AudioSource* pAudioSource{ pPlayer->AddComponent<that::AudioSource>() };
	pAudioSource->SetSound("walkmusic.wav");
	pAudioSource->SetLooping(true);

	constexpr int defaultHealth{ 3 };
	HealthComponent* pPlayerHealth{ pPlayer->AddComponent<HealthComponent>() };
	pPlayerHealth->SetMaxHealth(defaultHealth);
	pPlayerHealth->SetDestroyOnDeath(false);

	pPlayer->AddComponent<ScoreComponent>();

	// Pump
	that::GameObject* pPump{ pPlayer->CreateGameObject("Pump") };
	auto pPumpRenderer{ pPump->AddComponent<that::TextureRenderer>() };
	pPumpRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Pump.png"));
	pPumpRenderer->SetScale(2.0f);
	pPump->AddComponent<that::BoxCollider>();
	pPump->AddComponent<Pump>();
	pPump->AddComponent<that::TextureMask>()->SetPercentage(true, 1.0f);
	// Move the pump one cell to the right
	pPump->GetComponent<that::Transform>()->Translate(pGridComponent->GetCellSize() * 2.0f, 0.0f);


	// Input
	that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'a', 'w', 's' }, std::make_unique<GridMoveCommand>(pPlayer));
	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pPump));

	return pPlayer;
}

void digdug::GameScene::PrintControls()
{
	std::cout << "\n";
	std::cout << "Controls for DigDug Remade\n";
	std::cout << "\n";
	std::cout << "Move: WASD\n";
	std::cout << "Shoot pump: Space bar\n";
	std::cout << "\n";
	std::cout << "Sounds in the current game:\n";
	std::cout << "Music while walking\n";
	std::cout << "Sounds when you hit an enemy and a different sound when you kill the enemy\n";
	std::cout << "\n";
}
