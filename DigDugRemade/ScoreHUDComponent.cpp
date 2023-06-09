#include "ScoreHUDComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

#include <sstream>

void digdug::ScoreHUDComponent::SearchPlayer(that::GameObject* pPlayerParent, int playerIdx)
{
	m_pParent = pPlayerParent;
	m_PlayerIdx = playerIdx;
}

void digdug::ScoreHUDComponent::Display(that::GameObject* pPlayer)
{
	m_pScore = pPlayer->GetComponent<ScoreComponent>();
	m_pScore->OnScoreChange.AddListener(this);
	Notify(*m_pScore);
}

void digdug::ScoreHUDComponent::Update()
{
	if (!m_pParent) return;

	int curPlayerIdx{};
	for (that::GameObject* pChild : m_pParent->GetChildren())
	{
		if (pChild->GetTag() != "DigDug") continue;

		if (curPlayerIdx != m_PlayerIdx)
		{
			++curPlayerIdx;
			continue;
		}

		Display(pChild);
		m_pParent = nullptr;
		return;
	}
}

void digdug::ScoreHUDComponent::OnDestroy()
{
	if(m_pScore) m_pScore->OnScoreChange.RemoveListener(this);
}

void digdug::ScoreHUDComponent::OnSubjectDestroy()
{
	m_pScore = nullptr;
}

void digdug::ScoreHUDComponent::Notify(const ScoreComponent& score)
{
	GetOwner()->GetComponent<that::TextComponent>()->SetText(std::to_string(score.GetScore()));
}
