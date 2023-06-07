#include "ScoreComponent.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "GridTransform.h"

#include "EventQueue.h"

#include "ScorePrefab.h"

#include <iostream>

void digdug::ScoreComponent::AddScore(int score)
{
	m_Score += score;

	std::cout << m_Score << "\n";

	OnScoreChange.Notify(*this);
}

void digdug::ScoreComponent::RemoveScore(int score)
{
	m_Score -= score;

	OnScoreChange.Notify(*this);
}

void digdug::ScoreComponent::Init()
{
	that::EventQueue::GetInstance().AddListener(this);
}

void digdug::ScoreComponent::OnDestroy()
{
	that::EventQueue::GetInstance().RemoveListener(this);
}

void digdug::ScoreComponent::OnEvent(that::EntityDeathEvent* e)
{
	const float entityY{ e->pEntity->GetTransform()->GetLocalPosition().y };
	const int layerScore{ GetScoreForLayer(e->pEntity->GetComponent<GridTransform>()->GetCellPosition().y) };

	if (e->pEntity->GetTag() == "Pooka")
	{
		AddScore(layerScore);

		SpawnScore(GetOwner()->GetScene(), e->pEntity->GetTransform()->GetWorldPosition(), layerScore);
	}
	else if (e->pEntity->GetTag() == "Fygar")
	{
		const float playerY{ GetTransform()->GetLocalPosition().y };

		const bool horizontalAttack{ abs(playerY - entityY) <= m_FygarHorizontalEpsilon };

		const int fygarScore{ layerScore + horizontalAttack * layerScore };

		AddScore(fygarScore);

		SpawnScore(GetOwner()->GetScene(), e->pEntity->GetTransform()->GetWorldPosition(), fygarScore);
	}
}

int digdug::ScoreComponent::GetScoreForLayer(int height) const
{
	constexpr int baseScore{ 200 };
	constexpr int scorePerLayer{ 100 };
	constexpr int layerSize{ 3 };

	const int heightToUse{ std::max(height - 1, 0) };

	return baseScore + heightToUse / layerSize * scorePerLayer;
}
