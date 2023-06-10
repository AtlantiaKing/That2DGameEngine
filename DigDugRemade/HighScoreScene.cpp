#include "HighScoreScene.h"

#include "glm/vec2.hpp"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "TextureRenderer.h"
#include "TextComponent.h"
#include "HighScoreLetter.h"
#include "HighScoreButton.h"
#include "HighScoreInput.h"
#include "HighScoreSaver.h"

#include "TextureManager.h"
#include "ResourceManager.h"
#include "Window.h"
#include "GameData.h"

void digdug::HighScoreScene::Load(that::Scene& scene)
{
	const glm::ivec2& screenSize{ that::Window::GetInstance().GetSize() };

	// BACKGROUND
	that::GameObject* pBackground{ scene.CreateGameObject("Background") };
	const auto& pBackgroundTexture{ that::TextureManager::GetInstance().LoadTexture("HighScoreScreen/Background.png") };
	const auto pBackgroundRenderer{ pBackground->AddComponent<that::TextureRenderer>() };
	pBackgroundRenderer->SetTexture(pBackgroundTexture);

	const float uiScale{ static_cast<float>(screenSize.y) / pBackgroundTexture->GetSize().y };
	pBackground->GetTransform()->SetWorldScale(uiScale);
	pBackground->GetTransform()->SetLocalPosition(screenSize.x / 2.0f, screenSize.y / 2.0f);

	constexpr int m_FontSize{ 8 };
	const auto& pFont{ that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", m_FontSize) };

	// SCORE
	that::GameObject* pScore{ pBackground->CreateGameObject("Score") };
	pScore->AddComponent<that::TextureRenderer>();
	that::TextComponent* pScoreText{ pScore->AddComponent<that::TextComponent>() };
	pScoreText->SetFont(pFont);
	pScoreText->SetText(std::to_string(GameData::GetInstance().GetCurrentScores(0)));

	std::vector<HighScoreLetter*> pLetters{};
	for (size_t i{}; i < GameData::GetInstance().GetHighScoreUser().size(); ++i)
	{
		pLetters.push_back(CreateLetterEditor(pBackground, pFont, static_cast<int>(i)));
	}

	pScore->AddComponent<HighScoreSaver>()->SetLetters(pLetters);

	that::GameObject* pButtonObj{ pBackground->CreateGameObject("ReadyButton") };
	pButtonObj->AddComponent<that::TextureRenderer>()->SetTexture(that::TextureManager::GetInstance().LoadTexture("HighScoreScreen/Ready.png"));
	HighScoreButton* pButton{ pButtonObj->AddComponent<HighScoreButton>() };
	pButton->SetEnabled(false);
	pButtonObj->GetTransform()->SetLocalPosition(50.0f, 35.0f);

	that::GameObject* pInputObject{ scene.CreateGameObject("Input") };
	HighScoreInput* pInput{ pInputObject->AddComponent<HighScoreInput>() };
	pInput->SetLetters(pLetters);
	pInput->SetDoneButton(pButton);
}

digdug::HighScoreLetter* digdug::HighScoreScene::CreateLetterEditor(that::GameObject* pParent, std::shared_ptr<that::Font> pFont, int index)
{
	constexpr float distanceBetweenLetters{ 15.0f };
	constexpr float distanceBetweenLetterAndMarker{ 15.0f };
	constexpr float letterYPos{ 35.0f };
	const float letterXPos{ -distanceBetweenLetters + distanceBetweenLetters * index };

	that::GameObject* pLetter{ pParent->CreateGameObject("Letter") };
	pLetter->AddComponent<that::TextureRenderer>();
	that::TextComponent* pLetterText{ pLetter->AddComponent<that::TextComponent>() };
	pLetterText->SetFont(pFont);
	pLetterText->SetText("A");
	pLetter->GetTransform()->SetLocalPosition(letterXPos, letterYPos);
	HighScoreLetter* pLetterScript{ pLetter->AddComponent<HighScoreLetter>() };
	if(index > 0) pLetterScript->SetEnabled(false);
	pLetterScript->SetIndex(index);

	that::GameObject* pMarkerUp{ pParent->CreateGameObject("MarkerUp") };
	pMarkerUp->AddComponent<that::TextureRenderer>()->SetTexture(that::TextureManager::GetInstance().LoadTexture("HighScoreScreen/MarkerUp.png"));
	pMarkerUp->GetTransform()->SetLocalPosition(letterXPos, letterYPos - distanceBetweenLetterAndMarker);

	that::GameObject* pMarkerDown{ pParent->CreateGameObject("MarkerDown") };
	pMarkerDown->AddComponent<that::TextureRenderer>()->SetTexture(that::TextureManager::GetInstance().LoadTexture("HighScoreScreen/MarkerDown.png"));
	pMarkerDown->GetTransform()->SetLocalPosition(letterXPos, letterYPos + distanceBetweenLetterAndMarker);

	return pLetterScript;
}
