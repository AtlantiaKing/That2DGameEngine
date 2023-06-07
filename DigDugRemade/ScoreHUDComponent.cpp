#include "ScoreHUDComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

#include <sstream>

void digdug::ScoreHUDComponent::Display(that::GameObject* pPlayer)
{
	m_pScore = pPlayer->GetComponent<ScoreComponent>();
	m_pScore->OnScoreChange.AddListener(this);
}

void digdug::ScoreHUDComponent::OnDestroy()
{
	m_pScore->OnScoreChange.RemoveListener(this);
}

void digdug::ScoreHUDComponent::Notify(const ScoreComponent& score)
{
	std::stringstream hudText{};
	hudText << "Score: " << score.GetScore();

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}
