#include "SceneManager.h"
#include "Scene.h"

void that::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void that::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
		scene->UpdateCleanup();
	}
}

void that::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

that::Scene& that::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
