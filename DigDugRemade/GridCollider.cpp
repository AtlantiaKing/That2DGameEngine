#include "GridCollider.h"

void digdug::GridCollider::AddOnCollision(that::Observer<CollisionData>* observer)
{
	m_Subject.AddListener(observer);
}

void digdug::GridCollider::Hit(GridCollider* pOther)
{
	m_Subject.Notify(CollisionData{ this->GetOwner(), pOther->GetOwner() });
}
