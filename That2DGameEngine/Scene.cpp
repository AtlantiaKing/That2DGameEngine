#include "Scene.h"
#include "GameObject.h"

using namespace that;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

std::shared_ptr<GameObject> that::Scene::CreateGameObject()
{
	auto pGameObject{ std::make_shared<GameObject>() };

	m_objects.push_back(pGameObject);

	return pGameObject;
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}

	for (auto& object : m_objects)
	{
		object->UpdateCleanup();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

