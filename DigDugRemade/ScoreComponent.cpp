#include "ScoreComponent.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "GridTransform.h"
#include "Pump.h"
#include "DigDug.h"

#include "EventQueue.h"
#include "GameData.h"

#include "ScorePrefab.h"

void digdug::ScoreComponent::AddScore(int score)
{
	m_Score += score;

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

	m_Score = GameData::GetInstance().GetCurrentScores(GetOwner()->GetComponent<DigDug>()->GetPlayerIndex());

	OnScoreChange.Notify(*this);
}

void digdug::ScoreComponent::OnDestroy()
{
	GameData::GetInstance().SetCurrentScore(GetOwner()->GetComponent<DigDug>()->GetPlayerIndex(), m_Score);
	that::EventQueue::GetInstance().RemoveListener(this);
}

void digdug::ScoreComponent::OnEvent(that::EntityDeathEvent* e)
{
	Pump* pPump{ GetOwner()->GetChild(0)->GetComponent<Pump>() };
	that::GameObject* pTarget{ pPump->GetTarget() };

	if (!pTarget) return;
	if (pPump->GetTarget() != e->pEntity) return;

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
