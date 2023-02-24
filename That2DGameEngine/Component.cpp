#include "Component.h"

void that::Component::Destroy()
{
	m_IsMarkedDead = true;
}

void that::Component::SetParent(std::weak_ptr<GameObject> pParent)
{
	m_pParent = pParent;
}
