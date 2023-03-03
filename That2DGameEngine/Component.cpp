#include "Component.h"

void that::Component::Destroy()
{
	m_IsMarkedDead = true;
}

std::shared_ptr<that::Transform> that::Component::GetTransform() const
{
	if (m_pParent.expired()) return nullptr;

	return m_pParent.lock()->GetTransform();
}

std::shared_ptr<that::GameObject> that::Component::GetParent() const
{
	if (m_pParent.expired()) return nullptr;

	return m_pParent.lock();
}

void that::Component::SetParent(std::weak_ptr<GameObject> pParent) 
{
	m_pParent = pParent;
}
