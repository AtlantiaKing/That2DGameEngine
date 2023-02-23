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

void that::GameObject::Render() const 
{
	// Render every render component
	for(const auto& pRenderComponent : m_pRenderComponents)
	{
		pRenderComponent->Render();
	}
}
