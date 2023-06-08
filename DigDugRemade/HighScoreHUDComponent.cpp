#include "HighScoreHUDComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

#include "GameData.h"

#include <sstream>

void digdug::HighScoreHUDComponent::SearchPlayer(that::GameObject* pPlayerParent)
{
	m_pParent = pPlayerParent;
}

void digdug::HighScoreHUDComponent::Display(that::GameObject* pPlayer)
{
	m_pScore = pPlayer->GetComponent<ScoreComponent>();
	m_pScore->OnScoreChange.AddListener(this);
}

void digdug::HighScoreHUDComponent::Init()
{
	m_HighScore = GameData::GetInstance().GetHighScore();
	GetOwner()->GetComponent<that::TextComponent>()->SetText(std::to_string(m_HighScore));
}

void digdug::HighScoreHUDComponent::Update()
{
	if (!m_pParent) return;

	for (that::GameObject* pChild : m_pParent->GetChildren())
	{
		if (pChild->GetTag() != "DigDug") continue;

		Display(pChild);
		m_pParent = nullptr;
		return;
	}
}

void digdug::HighScoreHUDComponent::OnDestroy()
{
	if(m_pScore) m_pScore->OnScoreChange.RemoveListener(this);
}

void digdug::HighScoreHUDComponent::OnSubjectDestroy()
{
	m_pScore = nullptr;
}

void digdug::HighScoreHUDComponent::Notify(const ScoreComponent& score)
{
	const int newScore{ score.GetScore() };

	if(newScore > m_HighScore)
		GetOwner()->GetComponent<that::TextComponent>()->SetText(std::to_string(score.GetScore()));
}
