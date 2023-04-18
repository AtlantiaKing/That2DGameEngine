#include "ScoreComponent.h"

void digdug::ScoreComponent::AddScore(int score)
{
	m_Score += score;

	m_ScoreChangedListeners.Notify(*this);
}

void digdug::ScoreComponent::RemoveScore(int score)
{
	m_Score -= score;

	m_ScoreChangedListeners.Notify(*this);
}
