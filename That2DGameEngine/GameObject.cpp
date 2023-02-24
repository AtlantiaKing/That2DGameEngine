#include <string>
#include "GameObject.h"
#include "TextureRenderer.h"

that::GameObject::~GameObject() = default;

void that::GameObject::Update()
{
	// Update every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}

	// TODO: call FixedUpdate

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

void that::GameObject::Destroy(std::shared_ptr<Component> pComponent)
{
	// Mark the component as dead
	pComponent->Destroy();
}
