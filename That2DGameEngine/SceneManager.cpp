#include "SceneManager.h"
#include "Scene.h"

void that::SceneManager::LoadScene(unsigned int index)
{
	if (index >= m_SceneLoaders.size())
	{
		Logger::LogWarning("Trying to log a scene that is out of range");
		return;
	}

	// Remove the previous scene
	m_pScene = std::make_unique<Scene>();

	// Load the new scene
	m_SceneLoaders[index](*m_pScene);
}

void that::SceneManager::AddScene(const std::function<void(Scene&)>& sceneLoader)
{
	m_SceneLoaders.push_back(sceneLoader);
}

void that::SceneManager::Update()
{
	m_pScene->Update();
}

void that::SceneManager::LateUpdate()
{
	m_pScene->LateUpdate();
}

void that::SceneManager::UpdateCleanup()
{
	m_pScene->UpdateCleanup();
}

void that::SceneManager::Render()
{
	m_pScene->Render();
}

void that::SceneManager::OnGUI()
{
	m_pScene->OnGUI();
}
