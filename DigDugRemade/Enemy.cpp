#include "Enemy.h"

#include "EventQueue.h"
#include "GameObject.h"

void digdug::Enemy::Kill()
{
	that::EventQueue::GetInstance().SendEvent(EnemyDeathEvent{ GetOwner() });
	GetOwner()->Destroy();
}
