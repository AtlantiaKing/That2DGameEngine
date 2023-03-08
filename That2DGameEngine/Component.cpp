#include "Component.h"
#include "GameObject.h"

void that::Component::Destroy()
{
	m_IsMarkedDead = true;
}

that::Transform* that::Component::GetTransform() const
{
	if (m_pOwner.expired()) return nullptr;

	return m_pOwner.lock()->GetTransform();
}

std::shared_ptr<that::GameObject> that::Component::GetOwner() const
{
	if (m_pOwner.expired()) return nullptr;

	return m_pOwner.lock();
}

void that::Component::SetOwner(std::weak_ptr<GameObject> pParent) 
{
	m_pOwner = pParent;
}
