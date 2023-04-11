#include "Pump.h"

#include "GameObject.h"
#include "EnemyMovement.h"
#include "TextureRenderer.h"

#include "Timer.h"
#include "GridTransform.h"
#include "Player.h"
#include "Enemy.h"

void digdug::Pump::Init()
{
	GetOwner()->GetComponent<GridCollider>()->AddOnCollision(this);
	GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);
}

void digdug::Pump::Update()
{
	if (!m_IsActive) return;

	m_AccuAliveTime -= that::Timer::GetInstance().GetElapsed();
	if (m_AccuAliveTime <= 0)
	{
		m_IsActive = false;
		GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(false);
		GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(true);
	}
}

void digdug::Pump::Notify(const CollisionData& data)
{
	if (!m_IsActive) return;

	if (data.other->GetComponent<EnemyMovement>())
	{
		data.other->GetComponent<Enemy>()->Kill();
		GetOwner()->GetParent()->GetComponent<Player>()->AddScore();
	}
}

void digdug::Pump::Enable()
{
	if (m_IsActive) return;

	m_IsActive = true;
	GetOwner()->GetComponent<that::TextureRenderer>()->SetEnabled(true);
	GetOwner()->GetParent()->GetComponent<GridTransform>()->SetEnabled(false);
	m_AccuAliveTime = m_AliveTime;
}
