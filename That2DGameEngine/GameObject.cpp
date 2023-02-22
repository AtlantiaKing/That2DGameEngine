#include <string>
#include "GameObject.h"
#include "TextureRenderer.h"

that::GameObject::~GameObject() = default;

void that::GameObject::Update(){}

void that::GameObject::Render() const 
{
	for(const auto & pRenderComponent : m_pRenderComponents)
	{
		pRenderComponent->Render();
	}
}
