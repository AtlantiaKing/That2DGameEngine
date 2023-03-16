#include "Scene.h"
#include "GameObject.h"

using namespace that;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

GameObject* that::Scene::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>(this) };
	pGameObject->Init();

	GameObject* pGameObjectPtr{ pGameObject.get() };

	m_objects.push_back(std::move(pGameObject));

	return pGameObjectPtr;
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void that::Scene::Add(std::unique_ptr<GameObject> pGameObject)
{
	m_objects.push_back(std::move(pGameObject));
}

std::unique_ptr<GameObject> that::Scene::GetUnique(GameObject* pGameObject)
{
	for (auto it{ begin(m_objects) }; it < end(m_objects); ++it)
	{
		if (it->get() != pGameObject) continue;

		auto pUnique{ std::move(*it) };

		m_objects.erase(it);

		return pUnique;
	}

	return nullptr;
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void that::Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void that::Scene::UpdateCleanup()
{
	// Remove gameobjects from their container if they are marked as dead
	m_objects.erase(std::remove_if(begin(m_objects), end(m_objects), [](const auto& pGameObject)
		{
			return pGameObject->IsMarkedAsDead();
		}), end(m_objects));

	// Remove all components that are marked as dead
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

void that::Scene::OnGUI()
{
	for (const auto& object : m_objects)
	{
		object->OnGUI();
	}
}

