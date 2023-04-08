#include "Player.h"

#include "GameObject.h"
#include "GridCollider.h"
#include "EnemyMovement.h"
#include "GridTransform.h"

void digdug::Player::Init()
{
	GetOwner()->GetComponent<GridCollider>()->AddOnCollision(this);
}

void digdug::Player::Notify(const CollisionData& data)
{
	if (data.other->GetComponent<EnemyMovement>())
	{
		m_Health.Hit();
		GetOwner()->GetComponent<GridTransform>()->SetPosition(0, 0);
	}
}
