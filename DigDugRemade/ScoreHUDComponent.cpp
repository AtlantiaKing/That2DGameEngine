#include "ScoreHUDComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

#include <sstream>

void digdug::ScoreHUDComponent::SearchPlayer(that::GameObject* pPlayerParent)
{
	m_pParent = pPlayerParent;
}

void digdug::ScoreHUDComponent::Display(that::GameObject* pPlayer)
{
	m_pScore = pPlayer->GetComponent<ScoreComponent>();
	m_pScore->OnScoreChange.AddListener(this);
}

void digdug::ScoreHUDComponent::Update()
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

void digdug::ScoreHUDComponent::OnDestroy()
{
	m_pScore->OnScoreChange.RemoveListener(this);
}

void digdug::ScoreHUDComponent::Notify(const ScoreComponent& score)
{
	GetOwner()->GetComponent<that::TextComponent>()->SetText(std::to_string(score.GetScore()));
}
