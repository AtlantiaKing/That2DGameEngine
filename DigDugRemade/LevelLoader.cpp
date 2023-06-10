#include "LevelLoader.h"

// Objects
#include "GameObject.h"
#include "Surface2D.h"

// Components
#include "Transform.h"
#include "GridComponent.h"
#include "GridTransform.h"
#include "TextureRenderer.h"
#include "GridCollider.h"
#include "DigDug.h"
#include "LivesHUDComponent.h"
#include "TextComponent.h"
#include "Pump.h"
#include "ScoreHUDComponent.h"
#include "TextureMask.h"
#include "AudioSource.h"
#include "BoxCollider.h"
#include "WorldTile.h"
#include "Rigidbody.cpp"
#include "Pooka.h"
#include "Fygar.h"
#include "FireBreath.h"
#include "SpriteRenderer.h"

// Managers
#include "InputManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Window.h"

// Commands
#include "GridMoveCommand.h"
#include "ShootPumpCommand.h"
#include "PumpToEnemyCommand.h"

// STD includes
#include <sstream>

// Defines
#include "ColliderLayers.h"
#include "Rock.h"
#include "DisableOnGroundHit.h"

void digdug::LevelLoader::SetLevel(const std::string& filePath, int nrPlayers, bool playerFygar)
{
	m_Level = filePath;
	m_NrPlayers = nrPlayers;
	m_PlayerIsFygar = playerFygar;
}

void digdug::LevelLoader::Init()
{
	auto pGridComponent{ GetOwner()->AddComponent<digdug::GridComponent>() };

	const auto& pLevelData{ that::ResourceManager::GetInstance().LoadSurface(m_Level) };
	const auto& levelSize{ pLevelData->GetSize() };

	pGridComponent->SetSize(levelSize.x, levelSize.y);

	const float cellSize{ pGridComponent->GetCellSize() };

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

			std::stringstream tileFilePath{};
			tileFilePath << "Tiles/WorldTile" << (y - 1) / 3 << ".png";

			const auto& pWorldTileTexture{ that::TextureManager::GetInstance().LoadTexture(tileFilePath.str()) };
			pTile->AddComponent<that::TextureRenderer>()->SetTexture(pWorldTileTexture);

			pGridComponent->SetTile(x, y, pTile->AddComponent<WorldTile>());
		}
	}

	const glm::ivec2 m_MapSize{ pGridComponent->GetSize() * static_cast<int>(pGridComponent->GetCellSize()) };
	const auto& windowSize{ that::Window::GetInstance().GetSize() };

	const float scale{ static_cast<float>(windowSize.y) / m_MapSize.y };
	GetTransform()->SetWorldScale(scale);
	GetTransform()->Translate(cellSize / 2.0f * scale, cellSize / 2.0f * scale);
}

void digdug::LevelLoader::OnFrameStart()
{
	const auto& pLevelData{ that::ResourceManager::GetInstance().LoadSurface(m_Level) };
	const auto& levelSize{ pLevelData->GetSize() };

	GridComponent* pGridComponent{ GetOwner()->GetComponent<GridComponent>() };
	const float cellSize{ pGridComponent->GetCellSize() };

	std::vector<that::GameObject*> pPlayers{};

	for (int i{}; i < m_NrPlayers; ++i)
	{
		that::GameObject* pPlayer{ CreatePlayer(i) };
		pGridComponent->BindPlayer(pPlayer->GetComponent<GridTransform>());
		pPlayers.push_back(pPlayer);
	}

	// SURFACE PASS 2
	// Move all players
	int curPlayer{};
	for (int x{}; x < levelSize.x; ++x)
	{
		for (int y{}; y < levelSize.y; ++y)
		{
			const auto& color{ pLevelData->GetPixel(x,y) };

			if (!color.a)
			{
				pPlayers[curPlayer]->GetTransform()->SetLocalPosition(x * cellSize, y * cellSize);

				++curPlayer;

				if (curPlayer >= static_cast<int>(pPlayers.size())) break;
			}
		}
		if (curPlayer >= static_cast<int>(pPlayers.size())) break;
	}


	// SURFACE PASS 3
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

			if (enemyData)
			{
				that::GameObject* pEnemy{ GetOwner()->CreateGameObject("Enemy") };
				pEnemy->AddComponent<that::SpriteRenderer>();
				pEnemy->AddComponent<GridTransform>();
				that::BoxCollider* pCollider{ pEnemy->AddComponent<that::BoxCollider>() };
				pCollider->SetLayer(ENEMY_LAYER);
				pCollider->SetSize(cellSize, cellSize);
				pEnemy->GetComponent<that::Transform>()->SetLocalPosition(cellSize * x, cellSize * y);
				HealthComponent* pHealth{ pEnemy->AddComponent<HealthComponent>() };
				pHealth->SetMaxHealth(m_EnemyHealth);
				pHealth->SetDestroyOnDeath(false);
				if (enemyData == UINT8_MAX)
				{
					pEnemy->AddComponent<Pooka>()->Start(pPlayers);
					pEnemy->SetTag("Pooka");
				}
				else
				{
					pEnemy->AddComponent<Fygar>()->Start(pPlayers, m_PlayerIsFygar);

					// Disable player is fygar flag so the next fygar is an AI
					m_PlayerIsFygar = false;

					pEnemy->SetTag("Fygar");

					that::GameObject* pFire{ pEnemy->CreateGameObject("FireBreath") };
					that::TextureRenderer* pFireRenderer{ pFire->AddComponent<that::TextureRenderer>() };
					pFireRenderer->SetTexture(that::TextureManager::GetInstance().LoadTexture("Fygar/FireBreath.png"));
					pFire->GetTransform()->SetLocalPosition((cellSize + pFireRenderer->GetTextureSize().x) / 2.0f, 0.0f);
					pFire->AddComponent<FireBreath>();
					pFire->AddComponent<that::BoxCollider>();
					pFire->AddComponent<that::TextureMask>()->SetPercentage(true, 0.0f);
					pFire->SetActive(false);
					that::GameObject* pCollisionDetector{ pFire->CreateGameObject("CollisionDetection") };
					pCollisionDetector->AddComponent<DisableOnGroundHit>();
				}
			}

			const int rockData{ color.g };

			if (rockData)
			{
				that::GameObject* pRock{ GetOwner()->CreateGameObject("Rock") };
				pRock->AddComponent<that::SpriteRenderer>();
				pRock->GetTransform()->SetLocalPosition(x * cellSize, y * cellSize);
				that::BoxCollider* pCollider{ pRock->AddComponent<that::BoxCollider>() };
				pCollider->SetLayer(ROCK_LAYER);
				pCollider->SetSize(cellSize * 1.5f, cellSize * 1.5f);
				that::Rigidbody* pRb{ pRock->AddComponent<that::Rigidbody>() };
				pRb->SetGravityEnabled(false);
				pRb->SetConstantGravity(true);
				pRb->SetCollisionCorrection(false);
				pRb->SetGravity(300);
				pRock->AddComponent<Rock>()->Start(pPlayers);
			}
		}
	}

	// Destroy the level loader component from the grid
	Destroy();
}

that::GameObject* digdug::LevelLoader::CreatePlayer(int index)
{
	const float cellSize{ GetOwner()->GetComponent<GridComponent>()->GetCellSize()};

	// Player
	that::GameObject* pPlayer{ GetOwner()->CreateGameObject("Player") };
	pPlayer->SetTag("DigDug");
	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture("MainCharacter.png") };
	pPlayer->AddComponent<that::SpriteRenderer>();
	pPlayer->AddComponent<GridTransform>()->ShouldRotateWhenGoingUp(true);
	that::BoxCollider* pCollider{ pPlayer->AddComponent<that::BoxCollider>() };
	pCollider->SetSize(cellSize, cellSize);
	pCollider->SetLayer(DIGDUG_LAYER);
	pPlayer->AddComponent<DigDug>()->SetPlayerIndex(index);
	that::AudioSource* pAudioSource{ pPlayer->AddComponent<that::AudioSource>() };
	pAudioSource->SetSound("walkmusic.wav");
	pAudioSource->SetLooping(true);

	constexpr int defaultHealth{ 4 };
	HealthComponent* pPlayerHealth{ pPlayer->AddComponent<HealthComponent>() };
	pPlayerHealth->SetMaxHealth(defaultHealth);
	pPlayerHealth->SetDestroyOnDeath(false);

	pPlayer->AddComponent<ScoreComponent>();

	// Pump
	that::GameObject* pPump{ pPlayer->CreateGameObject("Pump") };
	auto pPumpRenderer{ pPump->AddComponent<that::TextureRenderer>() };
	const auto& pPumpTexture{ that::TextureManager::GetInstance().LoadTexture("Pump.png") };
	pPumpRenderer->SetTexture(pPumpTexture);
	pPump->AddComponent<that::BoxCollider>();
	pPump->AddComponent<Pump>();
	pPump->AddComponent<that::TextureMask>()->SetPercentage(true, 1.0f);
	// Move the pump one cell to the right
	pPump->GetComponent<that::Transform>()->Translate((pPlayerTexture->GetSize().x + pPumpTexture->GetSize().x) / 2.0f, 0.0f);
	that::GameObject* pCollisionDetector{ pPump->CreateGameObject("CollisionDetection") };
	pCollisionDetector->AddComponent<DisableOnGroundHit>();

	return pPlayer;
}