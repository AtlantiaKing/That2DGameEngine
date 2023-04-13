#include "GridCollider.h"

void digdug::GridCollider::Hit(GridCollider* pOther)
{
	m_Subject.Notify(CollisionData{ this->GetOwner(), pOther->GetOwner() });
}
