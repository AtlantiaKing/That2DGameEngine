#include "HighScoreTableScene.h"

#include "GameObject.h"

#include "TextureRenderer.h"
#include "Transform.h"
#include "TextComponent.h"
#include "BackToMainMenu.h"

#include "ResourceManager.h"
#include "GameData.h"
#include "Window.h"
#include "TextureManager.h"

#include <sstream>

void digdug::HighScoreTable::Load(that::Scene& scene)
{
	const auto& windowSize{ that::Window::GetInstance().GetSize() };

	// BACKGROUND
	that::GameObject* pBackground{ scene.CreateGameObject("Background") };
	const auto& pBackgroundTexture{ that::TextureManager::GetInstance().LoadTexture("HighScoreTable/Background.png") };
	const auto pBackgroundRenderer{ pBackground->AddComponent<that::TextureRenderer>() };
	pBackgroundRenderer->SetTexture(pBackgroundTexture);

	const float uiScale{ static_cast<float>(windowSize.y) / pBackgroundTexture->GetSize().y };
	pBackground->GetTransform()->SetWorldScale(uiScale);
	pBackground->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	pBackground->AddComponent<BackToMainMenu>();


	// HEADER
	that::GameObject* pHeader{ CreateText(scene, "Header", "HIGH SCORES") };
	pHeader->GetTransform()->SetWorldPosition(windowSize.x / 2.0f, windowSize.y * 0.3f);


	// HIGHSCORES
	const auto& highScores{ GameData::GetInstance().GetHighScores() };

	constexpr float spaceBetweenHighScores{ 25.0f };
	constexpr float spaceBetweenTexts{ 100.0f };
	constexpr float offset{ 20.0f };

	float curHighScorePos{ windowSize.y * 0.4f };
	int curPlacement{ 1 };
	for (auto it{ highScores.rbegin() }; it != highScores.rend(); ++it)
	{
		const auto& highScorePair{ *it };

		std::stringstream placementText{};
		placementText << curPlacement;
		switch (curPlacement)
		{
		case 1:
			placementText << "st";
			break;
		case 2:
		case 3:
			placementText << "nd";
			break;
		default:
			placementText << "th";
		}

		that::GameObject* pPlacement{ CreateText(scene, "Placement", placementText.str()) };
		pPlacement->GetComponent<that::TextureRenderer>()->SetPivot(1.0f, 0.5f);
		pPlacement->GetTransform()->SetWorldPosition(windowSize.x / 2.0f - spaceBetweenTexts + offset, curHighScorePos);

		that::GameObject* pScore{ CreateText(scene, "Score", std::to_string(highScorePair.first)) };
		pScore->GetComponent<that::TextureRenderer>()->SetPivot(1.0f, 0.5f);
		pScore->GetTransform()->SetWorldPosition(windowSize.x / 2.0f + offset, curHighScorePos);

		that::GameObject* pUser{ CreateText(scene, "Name", highScorePair.second) };
		pUser->GetComponent<that::TextureRenderer>()->SetPivot(1.0f, 0.5f);
		pUser->GetTransform()->SetWorldPosition(windowSize.x / 2.0f + spaceBetweenTexts + offset, curHighScorePos);

		curHighScorePos += spaceBetweenHighScores;
		++curPlacement;
	}
}

that::GameObject* digdug::HighScoreTable::CreateText(that::Scene& scene, const std::string& objectName, const std::string& text)
{
	const auto& pFont{ that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 16) };

	that::GameObject* pTextObj{ scene.CreateGameObject(objectName) };
	pTextObj->AddComponent<that::TextureRenderer>();
	that::TextComponent* pTextComponent{ pTextObj->AddComponent<that::TextComponent>() };
	pTextComponent->SetFont(pFont);
	pTextComponent->SetText(text);

	return pTextObj;
}
