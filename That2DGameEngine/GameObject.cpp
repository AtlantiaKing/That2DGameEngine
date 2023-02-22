#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

that::GameObject::~GameObject() = default;

void that::GameObject::Update(){}

void that::GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void that::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void that::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
