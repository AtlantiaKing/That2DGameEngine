#pragma once

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "GridComponent.h"
#include "GridTransform.h"
#include "TextureRenderer.h"

#include "InputManager.h"
#include "ResourceManager.h"

#include "GridMoveCommand.h"

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

		that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'q', 'z', 's' }, std::make_unique<GridMoveCommand>(pPlayer));
	}
}