#include "LevelLoader.h"

// Objects
#include "GameObject.h"
#include "Surface2D.h"

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
#include "WorldTile.h"
#include "Rigidbody.cpp"
#include "EnemyBehaviour.h"

// Managers
#include "InputManager.h"
#include "ResourceManager.h"

// Commands
#include "GridMoveCommand.h"
#include "ShootPumpCommand.h"

// STD includes
#include <sstream>
#include <iostream>

void digdug::LevelLoader::SetLevel(const std::string& filePath)
{
	m_Level = filePath;
}

void digdug::LevelLoader::Init()
{
	auto pGridComponent{ GetOwner()->AddComponent<digdug::GridComponent>()};

	const auto& pLevelData{ that::ResourceManager::GetInstance().LoadSurface(m_Level) };
	const auto& levelSize{ pLevelData->GetSize() };

	pGridComponent->SetSize(levelSize.x, levelSize.y);

	const float cellSize{ pGridComponent->GetCellSize() };
	const auto& pWorldTileTexture{ that::ResourceManager::GetInstance().LoadTexture("WorldTile.png") };

	// SURFACE PASS 1
	// Create all world tiles
	for (int x{}; x < levelSize.x; ++x)
	{
		for (int y{}; y < levelSize.y; ++y)
		{
			const int mapData{ pLevelData->GetPixel(x,y).r };
			if (mapData == m_NoMapValue) continue;

			that::GameObject* pTile{ GetOwner()->CreateGameObject("WorldTile") };

			pTile->GetTransform()->SetLocalPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize));

			auto pTexture{ pTile->AddComponent<that::TextureRenderer>() };
			pTexture->SetTexture(pWorldTileTexture);

			pGridComponent->SetTile(x, y, pTile->AddComponent<WorldTile>());
		}
	}
}

void digdug::LevelLoader::OnFrameStart()
{
	const auto& pLevelData{ that::ResourceManager::GetInstance().LoadSurface(m_Level) };
	const auto& levelSize{ pLevelData->GetSize() };

	GridComponent* pGridComponent{ GetOwner()->GetComponent<GridComponent>() };

	// SURFACE PASS 2
	// Create all entities
	// Set world tile masks
	for (int x{}; x < levelSize.x; ++x)
	{
		for (int y{}; y < levelSize.y; ++y)
		{
			const auto& color{ pLevelData->GetPixel(x,y) };

			const int mapData{ color.r };

			if (mapData != m_NoMapValue)
			{
				WorldTile* pTile{ pGridComponent->GetTile(x, y) };

				const bool changeLeftMask{ x > 0 && pLevelData->GetPixel(x - 1, y).r != UINT8_MAX };
				const bool changeRightMask{ x < levelSize.x - 1 && pLevelData->GetPixel(x + 1, y).r != UINT8_MAX };
				const bool changeBottomMask{ x < levelSize.y - 1 && pLevelData->GetPixel(x, y + 1).r != UINT8_MAX };
				const bool changeTopMask{ y > 0 && pLevelData->GetPixel(x, y - 1).r != UINT8_MAX };

				const float maskPercentage{ 1.0f - static_cast<float>(mapData) / UINT8_MAX };

				pTile->SetMasks(changeLeftMask * maskPercentage,
					changeRightMask * maskPercentage,
					changeBottomMask * maskPercentage,
					changeTopMask * maskPercentage);
			}

			const int enemyData{ color.b };
			const float cellSize{ pGridComponent->GetCellSize() };

			if (enemyData)
			{
				that::GameObject* pEnemy{ GetOwner()->CreateGameObject("Enemy") };
				auto pEnemyRenderer{ pEnemy->AddComponent<that::TextureRenderer>() };
				pEnemyRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
				pEnemy->AddComponent<EnemyMovement>();
				pEnemy->AddComponent<EnemyBehaviour>();
				pEnemy->AddComponent<GridTransform>();
				pEnemy->AddComponent<that::BoxCollider>();
				pEnemy->GetComponent<that::Transform>()->SetLocalPosition(cellSize * x, cellSize * y);
				pEnemy->AddComponent<HealthComponent>()->SetMaxHealth(m_EnemyHealth);
			}
		}
	}

	that::GameObject* pPlayer{ CreatePlayer() };

	pGridComponent->BindPlayer(pPlayer->GetComponent<GridTransform>());

	// Destroy the level loader component from the grid
	Destroy();
}

that::GameObject* digdug::LevelLoader::CreatePlayer()
{
	// Player
	that::GameObject* pPlayer{ GetOwner()->CreateGameObject("Player") };
	const auto& pPlayerTexture{ that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png") };
	pPlayer->AddComponent<that::TextureRenderer>()->SetTexture(pPlayerTexture);
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
	const auto& pPumpTexture{ that::ResourceManager::GetInstance().LoadTexture("Pump.png") };
	pPumpRenderer->SetTexture(pPumpTexture);
	pPump->AddComponent<that::BoxCollider>();
	pPump->AddComponent<Pump>();
	pPump->AddComponent<that::TextureMask>()->SetPercentage(true, 1.0f);
	// Move the pump one cell to the right
	pPump->GetComponent<that::Transform>()->Translate((pPlayerTexture->GetSize().x + pPumpTexture->GetSize().x) / 2.0f, 0.0f);


	// Input
	that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'a', 'w', 's' }, std::make_unique<GridMoveCommand>(pPlayer));
	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pPump));

	return pPlayer;
}