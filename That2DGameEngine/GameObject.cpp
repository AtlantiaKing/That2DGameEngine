#include <string>
#include "GameObject.h"
#include "TextureRenderer.h"

that::GameObject::~GameObject() = default;

void that::GameObject::Update()
{
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}

	// TODO: call FixedUpdate

	for (const auto& pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}
}

void that::GameObject::Render() const 
{
	for(const auto & pRenderComponent : m_pRenderComponents)
	{
		pRenderComponent->Render();
	}
}
