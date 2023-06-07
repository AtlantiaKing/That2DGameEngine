#include "Player.h"

#include "GameObject.h"

#include "BoxCollider.h"
#include "EnemyMovement.h"
#include "GridTransform.h"
#include "HealthComponent.h"
#include "AudioSource.h"

#include "EventQueue.h"

#include "ColliderLayers.h"

void digdug::Player::Init()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().AddListener(this);

	GridTransform* pGridTransform{ GetOwner()->GetComponent<GridTransform>() };
	that::AudioSource* pAudio{ GetOwner()->GetComponent<that::AudioSource>() };
	pGridTransform->OnStartMove.AddListener([pAudio](const auto&) { pAudio->ChangePlayState(false); });
	pGridTransform->OnStopMove.AddListener([pAudio](const auto&) { pAudio->ChangePlayState(true); });
}

void digdug::Player::OnDestroy()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().RemoveListener(this);
}

void digdug::Player::Notify(const that::CollisionData& data)
{
	if (data.pOther->GetLayer() == ENEMY_LAYER)
	{
		GetOwner()->GetComponent<GridTransform>()->SetPosition(0, 0);

		GetOwner()->GetComponent<HealthComponent>()->Hit();
		that::EventQueue::GetInstance().SendEvent(PlayerHitEvent{ GetOwner() });
	}
}