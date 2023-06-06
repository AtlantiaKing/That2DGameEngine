#include "MainMenuScene.h"

#include "TextureRenderer.h"
#include "Transform.h"

#include "Window.h"
#include "ResourceManager.h"
#include "GameModeChooser.h"
#include <iostream>
#include "MainMenuFadeIn.h"

void digdug::MainMenuScene::Load(that::Scene& scene)
{
	const glm::ivec2& screenSize{ that::Window::GetInstance().GetSize() };

	// BACKGROUND
	that::GameObject* pBackground{ scene.CreateGameObject("Background") };
	const auto& pBackgroundTexture{ that::ResourceManager::GetInstance().LoadTexture("MainMenu/Background.png") };
	const auto pBackgroundRenderer{ pBackground->AddComponent<that::TextureRenderer>() };
	pBackgroundRenderer->SetTexture(pBackgroundTexture);

	const float uiScale{ static_cast<float>(screenSize.y) / pBackgroundTexture->GetSize().y};
	pBackground->GetTransform()->SetWorldScale(uiScale);

	// MARKER
	that::GameObject* pMarker{ pBackground->CreateGameObject("Marker")};
	const auto& pMarkerTexture{ that::ResourceManager::GetInstance().LoadTexture("MainMenu/Marker.png") };
	const auto pMarkerRenderer{ pMarker->AddComponent<that::TextureRenderer>() };
	pMarkerRenderer->SetTexture(pMarkerTexture);
	const float markerPosX{ -45.0f };
	const auto pGameChooser{ pMarker->AddComponent<GameModeChooser>() };
	pGameChooser->SetMarkerPositions(
		{ markerPosX, -6.0f },
		{ markerPosX, 10.0f },
		{ markerPosX, 26.0f }
	);
	pMarker->SetActive(false);


	// FADE
	pBackground->GetTransform()->SetWorldPosition(screenSize.x / 2.0f, screenSize.y / 2.0f * 3.0f); // Spawn the background outside of the screen
	pBackground->AddComponent<MainMenuFadeIn>()->OnFadeComplete.AddListener([=](const MainMenuFadeIn&) 
		{ 
			pMarker->SetActive(true); 
			pGameChooser->BindInput();
		});
}
