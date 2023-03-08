#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Scene.h"

that::GameObject::~GameObject() = default;

that::GameObject* that::GameObject::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>(m_pScene) };
	pGameObject->Init();

	auto pGameObjectPtr{ pGameObject.get() };

	pGameObject->m_pParent = this;
	m_pChildren.push_back(std::move(pGameObject));

	return pGameObjectPtr;
}

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

	// Update every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void that::GameObject::LateUpdate()
{
	// LateUpdate every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}

	// LateUpdate every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->LateUpdate();
	}
}

void that::GameObject::UpdateCleanup()
{
	// Remove components from their containers if they are marked as dead
	m_pComponents.erase(std::remove_if(begin(m_pComponents), end(m_pComponents), [](const auto& pComponent)
		{
			return pComponent->IsMarkedAsDead();
		}), end(m_pComponents));

	// Remove the render component if its marked as dead
	if (m_pRenderComponent && m_pRenderComponent->IsMarkedAsDead()) m_pRenderComponent = nullptr;

	// Remove children from their containers if they are marked as dead
	m_pChildren.erase(std::remove_if(begin(m_pChildren), end(m_pChildren), [](const auto& pChild)
		{
			return pChild->IsMarkedAsDead();
		}), end(m_pChildren));

	// Cleanup every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->UpdateCleanup();
	}
}

void that::GameObject::Render() const 
{
	if(m_pRenderComponent) m_pRenderComponent->Render();

	// Render every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->Render();
	}
}

void that::GameObject::SetParent(GameObject* pParent)
{
	// Cache the previous parent
	GameObject* pOldParent{ m_pParent };

	// If this GO has a parent
	if (pOldParent)
	{
		// If the newly assigned parent is the same as the current parent, do nothing
		if (pOldParent == pParent) return;

		// Remove itself from the children list of the previous parent
		for (int i{ static_cast<int>(pOldParent->m_pChildren.size() - 1) }; i >= 0; --i)
		{
			auto& pChild{ pOldParent->m_pChildren[i] };

			if (pChild.get() == this)
			{
				// Move itself to the children list of the new parent
				if (pParent) pParent->m_pChildren.push_back(std::move(pChild));
				else m_pScene->Add(std::move(pChild));

				// Remove the dangling unique ptr
				pOldParent->m_pChildren[i] = std::move(pOldParent->m_pChildren[pOldParent->m_pChildren.size() - 1]);
				pOldParent->m_pChildren.pop_back();
				break;
			}
		}
	}
	else
	{
		// If there is no previous parent, and the new parent is also null, do nothing
		if (!pParent) return;
		
		pParent->m_pChildren.push_back(std::move(m_pScene->GetUnique(this)));
	}

	// Set the parent of this GO
	m_pParent = pParent;

	// Get the transform
	auto pTransform{ GetTransform() };
	if (!pTransform) return;

	// If a new parent is assigned
	if (pParent)
	{
		// Set the local position to the position relative to the new parent
		auto pParentTransform{ pParent->GetTransform() };
		if (pParentTransform) pTransform->SetLocalPosition(pTransform->GetWorldPosition() - pParentTransform->GetWorldPosition());
	}
	else // The parent got removed
	{
		// Set the local position to the world position
		pTransform->SetLocalPosition(pTransform->GetWorldPosition());
	}
}

that::GameObject* that::GameObject::GetChild(int index) const
{
	// TODO: Log a warning that the user tries to get a child out of bounds
	if (index >= static_cast<int>(m_pChildren.size())) return nullptr;

	return m_pChildren[index].get();
}

std::vector<that::GameObject*> that::GameObject::GetChildren() const
{
	std::vector<GameObject*> pGameObjects{};
	pGameObjects.reserve(m_pChildren.size());

	for (const auto& pChild : m_pChildren)
	{
		pGameObjects.emplace_back(pChild.get());
	}

	return pGameObjects;
}

void that::GameObject::Destroy()
{
	m_IsMarkedDead = true;

	// Destroy all children
	for (const auto& child : m_pChildren)
	{
		child->Destroy();
	}
}

bool that::GameObject::Destroy(Component* pComponent)
{
	// If the owner of this component is not this GO, do nothing
	if (pComponent->GetOwner() != this) return false;
	// TODO: log warning that the user is trying to remove a component that is not on this GO

	// Mark the component as dead
	pComponent->Destroy();

	return true;
}
