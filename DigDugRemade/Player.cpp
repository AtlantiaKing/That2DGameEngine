#include "Player.h"

#include "GameObject.h"
#include "GridCollider.h"
#include "EnemyMovement.h"
#include "GridTransform.h"

#include "EventQueue.h"

void digdug::Player::Init()
{
	GetOwner()->GetComponent<GridCollider>()->AddOnCollision(this);
}

void digdug::Player::Notify(const CollisionData& data)
{
	if (data.other->GetComponent<EnemyMovement>())
	{
		GetOwner()->GetComponent<GridTransform>()->SetPosition(0, 0);

		m_Health.Hit();
		that::EventQueue::GetInstance().SendEvent(PlayerHitEvent{ GetOwner() });
	}
}

void digdug::Player::AddScore()
{
	constexpr int scoreAddition{ 200 };
	m_Score += scoreAddition;
}
