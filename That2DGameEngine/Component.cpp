#include "Component.h"

that::Component::Component(std::weak_ptr<GameObject> pParent)
	: m_pParent{ pParent }
{
}
