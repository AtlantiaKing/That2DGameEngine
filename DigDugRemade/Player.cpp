#include "Player.h"

#include "GameObject.h"

#include "BoxCollider.h"
#include "EnemyMovement.h"
#include "GridTransform.h"
#include "HealthComponent.h"

#include "EventQueue.h"

void digdug::Player::Init()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().AddListener(this);
}

void digdug::Player::OnDestroy()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().RemoveListener(this);
}

void digdug::Player::Notify(const that::CollisionData& data)
{
	if (data.pOther->GetOwner()->GetComponent<EnemyMovement>())
	{
		GetOwner()->GetComponent<GridTransform>()->SetPosition(0, 0);

		GetOwner()->GetComponent<HealthComponent>()->Hit();
		that::EventQueue::GetInstance().SendEvent(PlayerHitEvent{ GetOwner() });
	}
}