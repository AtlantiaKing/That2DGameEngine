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

#include "InputManager.h"
#include "ResourceManager.h"

#include "GridMoveCommand.h"

#include <sstream>

namespace digdug
{
	void LoadGameScene(that::Scene& scene)
	{
		that::GameObject* pGrid{ scene.CreateGameObject("Grid") };
		auto pGridComponent{ pGrid->AddComponent<digdug::GridComponent>() };
		pGridComponent->SetCellSize(32.0f);
		pGrid->GetTransform()->SetWorldPosition(100, 40);

		that::GameObject* pPlayer{ pGrid->CreateGameObject("Player") };
		auto pPlayerRenderer{ pPlayer->AddComponent<that::TextureRenderer>() }; 
		pPlayerRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));
		pPlayerRenderer->SetScale(2.0f);
		pPlayer->AddComponent<digdug::GridTransform>();
		pPlayer->AddComponent<digdug::GridCollider>();
		Player* pPlayerComponent{ pPlayer->AddComponent<digdug::Player>() };

		that::GameObject* pEnemy{ pGrid->CreateGameObject("Enemy") };
		auto pEnemyRenderer{ pEnemy->AddComponent<that::TextureRenderer>() };
		pEnemyRenderer->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
		pEnemyRenderer->SetScale(2.0f);
		pEnemy->AddComponent<digdug::GridTransform>()->SetPosition(10, 10);
		pEnemy->AddComponent<digdug::EnemyMovement>();
		pEnemy->AddComponent<digdug::GridCollider>();

		that::GameObject* pLivesHUD{ scene.CreateGameObject("LivesHUD") };
		pLivesHUD->GetTransform()->SetWorldPosition(0.0f, 0.0f);
		pLivesHUD->AddComponent<digdug::LivesHUDComponent>();
		that::TextComponent* pLivesText{ pLivesHUD->AddComponent<that::TextComponent>() };
		pLivesText->SetFont(that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));

		std::stringstream hudText{};
		hudText << pPlayerComponent->GetHealth() << " lifes left";
		pLivesText->SetText(hudText.str());

		pLivesHUD->AddComponent<that::TextureRenderer>();

		that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'q', 'z', 's' }, std::make_unique<GridMoveCommand>(pPlayer));
	}
}