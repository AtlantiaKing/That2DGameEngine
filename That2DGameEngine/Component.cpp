#include "Component.h"
#include "GameObject.h"

void that::Component::Destroy()
{
	m_IsMarkedDead = true;
}

that::Transform* that::Component::GetTransform() const
{
	if (!m_pOwner) return nullptr;

	return m_pOwner->GetTransform();
}

void that::Component::SetOwner(GameObject* pParent) 
{
	m_pOwner = pParent;
}
