#include "Enemy.h"

#include "EventQueue.h"

digdug::Enemy::~Enemy()
{
	that::EventQueue::GetInstance().SendEvent(EnemyDeathEvent{ GetOwner() });
}
