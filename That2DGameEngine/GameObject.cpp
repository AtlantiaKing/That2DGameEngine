#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Scene.h"
#include "Logger.h"
#include "ComponentDeleter.h"

that::GameObject::GameObject(Scene* pScene, const std::string& name)
	: m_pScene{ pScene }
	, m_Name{ name }
	, m_pChildrenToAdd{}
	, m_pChildren{}
	, m_pComponentsToAdd{}
	, m_pComponents{}
{
	m_pTransform = AddComponent<Transform>();
}

that::GameObject* that::GameObject::CreateGameObject(const std::string& name)
{
	// Create a new gameobject
	auto pGameObject{ std::make_unique<GameObject>(m_pScene, name) };

	// Enable the dirty flag
	pGameObject->GetTransform()->EnableChangedFlag();

	// Store the raw pointer
	auto pGameObjectPtr{ pGameObject.get() };

	// Set the parent and add the gameobject to the children list
	pGameObject->m_pParent = this;
	m_pChildrenToAdd.push_back(std::move(pGameObject));

	// Return the raw pointer to the new gameobject
	return pGameObjectPtr;
}

void that::GameObject::Init()
{
	// Set the initialized flag
	m_Initialized = true;

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

	// Add any new components that have been added last frame
	if (!m_pComponentsToAdd.empty())
	{
		std::vector<std::unique_ptr<Component, ComponentDeleter>> pNewComponents{ std::move(m_pComponentsToAdd) };

		for (auto& pComponent : pNewComponents)
		{
			pComponent->Init();
			m_pComponents.push_back(std::move(pComponent));
		}
	}

	// Add any new children that have been added last frame
	if (!m_pChildrenToAdd.empty())
	{
		std::vector<std::unique_ptr<GameObject>> pNewChildren{ std::move(m_pChildrenToAdd) };

		for (auto& pChild : pNewChildren)
		{
			pChild->Init();
			m_pChildren.push_back(std::move(pChild));
		}
	}

	// Call OnFrameStart on every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->OnFrameStart();
	}

	// Call OnFrameStart on every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnFrameStart();
	}
}

void that::GameObject::Update()
{
	// Don't update if the gameobject is not active
	if (!m_IsActive) return;

	// Cache the active state
	const bool wasActive{ m_IsActive };

	// Set the update flag
	m_IsUpdating = true; 

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

	// Set the update flag
	m_IsUpdating = false;

	// Call OnDisable if the gameobject has been disabled during the loop
	if (wasActive != m_IsActive)
	{
		OnDisable();
	}
}

void that::GameObject::LateUpdate()
{
	// Don't update if the gameobject is not active
	if (!m_IsActive) return;

	// Cache the active state
	const bool wasActive{ m_IsActive };

	// Set the update flag
	m_IsUpdating = true;

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

	// Set the update flag
	m_IsUpdating = false;

	// Call OnDisable if the gameobject has been disabled during the loop
	if (wasActive != m_IsActive)
	{
		OnDisable();
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

void that::GameObject::OnDestroy() const
{
	// OnDestroy every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->OnDestroy();
	}

	// OnDestroy every child
	for (const auto& pChild : m_pChildren)
	{
		pChild->OnDestroy();
	}
}

void that::GameObject::OnGUI() const
{
	// Don't render if the gameobject is not active
	if (!m_IsActive) return;

	// Render GUI every component
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

void that::GameObject::OnEnable() const
{
	// Don't call OnDisable and OnEnable when inside the update loop
	if (m_IsUpdating) return;

	// OnEnable every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->OnEnable();
	}

	// OnEnable every child
	for (const auto& pChild : m_pChildren)
	{
		if (pChild->IsActive()) pChild->OnEnable();
	}
}

void that::GameObject::OnDisable() const
{
	// Don't call OnDisable and OnEnable when inside the update loop
	if (m_IsUpdating) return;

	// OnDisable every component
	for (const auto& pComponent : m_pComponents)
	{
		if (pComponent->IsEnabled()) pComponent->OnDisable();
	}

	// OnDisable every child
	for (const auto& pChild : m_pChildren)
	{
		if (pChild->IsActive()) pChild->OnDisable();
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
	// Create a new list of gameobjects
	std::vector<GameObject*> pGameObjects{};
	pGameObjects.reserve(m_pChildren.size() + m_pChildrenToAdd.size());

	// Add all the raw pointers to the children and the children to be added
	for (const auto& pChild : m_pChildren)
	{
		pGameObjects.emplace_back(pChild.get());
	}
	for (const auto& pChild : m_pChildrenToAdd)
	{
		pGameObjects.emplace_back(pChild.get());
	}

	return pGameObjects;
}

std::vector<that::Component*> that::GameObject::GetComponents() const
{
	// Create a new list of components
	std::vector<Component*> pComponents{};
	pComponents.reserve(m_pComponents.size() + m_pComponentsToAdd.size());

	// Add all the raw pointers to the components
	for (const auto& pComponent : m_pComponents)
	{
		pComponents.emplace_back(pComponent.get());
	}
	for (const auto& pComponent : m_pComponentsToAdd)
	{
		pComponents.emplace_back(pComponent.get());
	}

	return pComponents;
}

void that::GameObject::SetActive(bool isActive)
{
	// Don't call any methods when the active flag stayed the same
	if (m_IsActive == isActive) return;

	// Save the current active flag
	m_IsActive = isActive;

	// Call OnEnable/OnDisable
	if (m_IsActive)
		OnEnable();
	else
		OnDisable();
}

bool that::GameObject::IsActive() const
{
	return m_IsActive;
}

void that::GameObject::Destroy()
{
	// Mark this gameobject as destroyed
	m_IsMarkedDead = true;

	// Mark all children as destroyed
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

that::GameObject::~GameObject()
{

}