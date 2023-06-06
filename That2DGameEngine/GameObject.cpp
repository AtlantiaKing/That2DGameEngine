#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Scene.h"
#include "Logger.h"

that::GameObject::GameObject(Scene* pScene, const std::string& name)
	: m_pScene{ pScene }
	, m_Name{ name }
{
	m_pTransform = AddComponent<Transform>();
}

that::GameObject::~GameObject() = default;

that::GameObject* that::GameObject::CreateGameObject(const std::string& name)
{
	auto pGameObject{ std::make_unique<GameObject>(m_pScene, name) };

	if (m_pTransform->IsDirty()) pGameObject->GetTransform()->EnableChangedFlag();

	auto pGameObjectPtr{ pGameObject.get() };

	pGameObject->m_pParent = this;
	m_pChildrenToAdd.push_back(std::move(pGameObject));

	return pGameObjectPtr;
}

void that::GameObject::Init()
{
	// Initialize every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Init();
	}
}

void that::GameObject::OnFrameStart()
{
	// Don't update if the gameobject is not active
	if (!m_IsActive) return;

	for (auto& pChild : m_pChildrenToAdd)
	{
		pChild->Init();
		m_pChildren.push_back(std::move(pChild));
	}
	m_pChildrenToAdd.clear();

	// Update every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->OnFrameStart();
	}

	// Update every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnFrameStart();
	}
}

void that::GameObject::Update()
{
	// Don't update if the gameobject is not active
	if (!m_IsActive) return;

	// Update every component
	for (const auto& pComponent : m_pComponents)
	{
		if(pComponent->IsEnabled()) pComponent->Update();
	}

	// Update every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void that::GameObject::LateUpdate()
{
	// Don't update if the gameobject is not active
	if (!m_IsActive) return;

	// LateUpdate every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->LateUpdate();
	}

	// LateUpdate every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->LateUpdate();
	}
}

void that::GameObject::UpdateCleanup()
{
	// Cleanup every child (also calls OnDestroy on all child objects/components)
	for (const auto& pChild : m_pChildren)
	{
		pChild->UpdateCleanup();
	}

	// Call OnDestroy on all to-be-destructed components
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsMarkedAsDead()) pComponent->OnDestroy();
	}

	// Remove components from their container if they are marked as dead
	m_pComponents.erase(
		std::remove_if(
			begin(m_pComponents), 
			end(m_pComponents), 
			[](const auto& pComponent) { return pComponent->IsMarkedAsDead(); }
		), 
		end(m_pComponents));

	// Call OnDestroy on all to-be-destructed children
	for (const auto& pChild : m_pChildren)
	{
		if (pChild->IsMarkedAsDead()) pChild->OnDestroy();
	}

	// Remove components from their container if they are marked as dead
	m_pChildren.erase(
		std::remove_if(
			begin(m_pChildren), 
			end(m_pChildren), 
			[](const auto& pChild) { return pChild->IsMarkedAsDead(); }
		), 
		end(m_pChildren));
}

void that::GameObject::Render() const 
{
	// Don't render if the gameobject is not active
	if (!m_IsActive) return;

	// Render every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->Render();
	}

	// Render every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->Render();
	}
}

void that::GameObject::OnDestroy()
{
	// Render every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->OnDestroy();
	}

	// Render every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnDestroy();
	}
}

void that::GameObject::OnGUI()
{
	// LateUpdate every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->OnGUI();
	}

	// Render GUI every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnGUI();
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
	// Log a warning if the child is out of bounds
	if (index >= static_cast<int>(m_pChildren.size()))
	{
		Logger::LogWarning( "Trying to get a child with an index that is out of bounds", this);
		return nullptr;
	}

	return m_pChildren[index].get();
}

std::vector<that::GameObject*> that::GameObject::GetChildren() const
{
	std::vector<GameObject*> pGameObjects{};
	pGameObjects.reserve(m_pChildren.size() + m_pChildrenToAdd.size());

	for (const auto& pChild : m_pChildren)
	{
		pGameObjects.emplace_back(pChild.get());
	}

	return pGameObjects;
}

void that::GameObject::SetActive(bool isActive)
{
	m_IsActive = isActive;
}

bool that::GameObject::IsActive() const
{
	return m_IsActive;
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
	// Log a warning if the owner of this component is not this GO
	if (pComponent->GetOwner() != this)
	{
		Logger::LogWarning( "Trying to destroy a component that is not part of this gameobject.", this);
		return false;
	}

	// Mark the component as dead
	pComponent->Destroy();

	return true;
}
