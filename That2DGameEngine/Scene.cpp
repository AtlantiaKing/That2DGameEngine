#include "Scene.h"
#include "GameObject.h"

using namespace that;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

std::shared_ptr<GameObject> that::Scene::CreateGameObject()
{
	auto pGameObject{ std::make_shared<GameObject>() };
	pGameObject->Init();

	m_objects.push_back(pGameObject);

	return pGameObject;
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	object->Destroy();
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

	// TODO: Add FixedUpdate

	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}

	// Remove all components that are marked as dead
	for (auto& object : m_objects)
	{
		object->UpdateCleanup();
	}

	// Remove gameobjects from their container if they are marked as dead
	m_objects.erase(std::remove_if(begin(m_objects), end(m_objects), [](auto pGameObject)
		{
			return pGameObject->IsMarkedAsDead();
		}), end(m_objects));
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

