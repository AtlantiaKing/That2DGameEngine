#include "MainMenuScene.h"

#include "UIComponent.h"
#include "Transform.h"
#include "TextComponent.h"
#include "MainMenuFadeIn.h"
#include "GameModeChooser.h"

#include "Window.h"
#include "TextureManager.h"
#include "GameData.h"
#include "ResourceManager.h"

void digdug::MainMenuScene::Load(that::Scene& scene)
{
	const glm::ivec2& screenSize{ that::Window::GetInstance().GetSize() };

	// BACKGROUND
	that::GameObject* pBackground{ scene.CreateGameObject("Background") };
	const auto& pBackgroundTexture{ that::TextureManager::GetInstance().LoadTexture("MainMenu/Background.png") };
	const auto pBackgroundRenderer{ pBackground->AddComponent<that::UIComponent>() };
	pBackgroundRenderer->SetTexture(pBackgroundTexture);

	const float uiScale{ static_cast<float>(screenSize.y) / pBackgroundTexture->GetSize().y};
	pBackground->GetTransform()->SetWorldScale(uiScale);


	// HIGH SCORE
	const auto& pFont{ that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 8) };

	that::GameObject* pHighScore{ pBackground->CreateGameObject("HighScore") };
	pHighScore->AddComponent<that::UIComponent>()->SetPivot({ 1.0f, 0.5f });
	that::TextComponent* pHighScoreText{ pHighScore->AddComponent<that::TextComponent>() };
	pHighScoreText->SetFont(pFont);
	pHighScoreText->SetText(std::to_string(GameData::GetInstance().GetHighScore()));
	pHighScoreText->GetTransform()->SetLocalPosition(7.0f, -85.0f);

	that::GameObject* pHighScoreUser{ pBackground->CreateGameObject("HighScoreUser") };
	pHighScoreUser->AddComponent<that::UIComponent>()->SetPivot({ 1.0f, 0.5f });
	that::TextComponent* pHighScoreUserText{ pHighScoreUser->AddComponent<that::TextComponent>() };
	pHighScoreUserText->SetFont(pFont);
	pHighScoreUserText->SetText(GameData::GetInstance().GetHighScoreUser());
	pHighScoreUserText->GetTransform()->SetLocalPosition(74.0f, -85.0f);


	// MARKER
	that::GameObject* pMarker{ pBackground->CreateGameObject("Marker")};
	const auto& pMarkerTexture{ that::TextureManager::GetInstance().LoadTexture("MainMenu/Marker.png") };
	const auto pMarkerRenderer{ pMarker->AddComponent<that::UIComponent>() };
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
	pBackground->GetTransform()->SetLocalPosition(screenSize.x / 2.0f, screenSize.y / 2.0f * 3.0f); // Spawn the background outside of the screen
	pBackground->AddComponent<MainMenuFadeIn>()->OnFadeComplete.AddListener([=](const MainMenuFadeIn&) 
		{ 
			pMarker->SetActive(true); 
			pGameChooser->BindInput();
		});
}
