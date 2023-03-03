#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"

that::GameObject::~GameObject() = default;

void that::GameObject::Init()
{
	m_pTransform = AddComponent<Transform>();
}

void that::GameObject::Update()
{
	// Update every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void that::GameObject::LateUpdate()
{
	// LateUpdate every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}
}

void that::GameObject::UpdateCleanup()
{
	// Remove components from their containers if they are marked as dead
	m_pComponents.erase(std::remove_if(begin(m_pComponents), end(m_pComponents), [](auto pComponent)
		{
			return pComponent->IsMarkedAsDead();
		}), end(m_pComponents));
	m_pRenderComponents.erase(std::remove_if(begin(m_pRenderComponents), end(m_pRenderComponents), [](auto pComponent)
		{
			return pComponent->IsMarkedAsDead();
		}), end(m_pRenderComponents));
}

void that::GameObject::Render() const 
{
	// Render every render component
	for(const auto& pRenderComponent : m_pRenderComponents)
	{
		pRenderComponent->Render();
	}
}

void that::GameObject::SetParent(std::shared_ptr<GameObject> pParent)
{
	// If this GO has a parent
	if (!m_pParent.expired())
	{
		auto pOldParent{ m_pParent.lock() };

		// If the newly assigned parent is the same as the current parent, do nothing
		if (pOldParent == pParent) return;

		// Remove itself from the children list of the previous parent
		for (int i{ static_cast<int>(pParent->m_pChildren.size() - 1) }; i >= 0; --i)
		{
			auto pChild{ pParent->m_pChildren[i].lock() };

			if (pChild.get() == this)
			{
				pParent->m_pChildren[i] = pParent->m_pChildren[pParent->m_pChildren.size() - 1];
				pParent->m_pChildren.pop_back();
				break;
			}
		}
	}

	// Set the parent of this GO
	m_pParent = pParent;

	// Add itself to the children list of the new parent
	pParent->m_pChildren.push_back(weak_from_this());

	// TODO: Update transform
}

void that::GameObject::Destroy()
{
	m_IsMarkedDead = true;
}

void that::GameObject::Destroy(std::shared_ptr<Component> pComponent)
{
	// Mark the component as dead
	pComponent->Destroy();
}
