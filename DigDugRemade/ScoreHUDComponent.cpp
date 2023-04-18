#include "ScoreHUDComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

#include <sstream>

void digdug::ScoreHUDComponent::Display(that::GameObject* pPlayer)
{
	pPlayer->GetComponent<ScoreComponent>()->OnScoreChange().AddListener(this);
}

void digdug::ScoreHUDComponent::Notify(const ScoreComponent& score)
{
	std::stringstream hudText{};
	hudText << "Score: " << score.GetScore();

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}
