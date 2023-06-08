#include "SceneManager.h"
#include "Scene.h"

void that::SceneManager::LoadScene(unsigned int index)
{
	if (index >= m_SceneLoaders.size())
	{
		Logger::LogWarning("Trying to load a scene that is out of range");
		return;
	}

	// Set the scene for the next frame
	m_NextScene = index;
}

void that::SceneManager::AddScene(const std::function<void(Scene&)>& sceneLoader)
{
	m_SceneLoaders.push_back(sceneLoader);
}

void that::SceneManager::OnFrameStart()
{
	if (m_NextScene >= 0)
	{
		// Remove the previous scene
		m_pScene = std::make_unique<Scene>();

		// Load the new scene
		m_SceneLoaders[m_NextScene](*m_pScene);

		// Reset next scene
		m_NextScene = -1;
	}

	m_pScene->OnFrameStart();
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

void that::SceneManager::Destroy()
{
	m_pScene->Destroy();
}
