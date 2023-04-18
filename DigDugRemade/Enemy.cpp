#include "Enemy.h"

#include "EventQueue.h"
#include "Events.h"

#include "GameObject.h"

void digdug::Enemy::Kill()
{
	GetOwner()->Destroy();
}
