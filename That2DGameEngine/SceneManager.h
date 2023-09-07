#pragma once
#include "Singleton.h"

#include <vector>
#include <memory>
#include <functional>

namespace that
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void LoadScene(unsigned int index);
		void AddScene(const std::function<void(Scene&)>& sceneLoader);

		void OnFrameStart();
		void Update();
		void LateUpdate();
		void UpdateCleanup();
		void Render();
		void OnGUI();
		void Destroy();

		int GetCurrentSceneIndex() const { return m_CurrentScene; }
		Scene* GetCurrentScene() const { return m_pScene.get(); }
		
		void DrawSceneHierarchy(bool drawDebug);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::function<void(Scene&)>> m_SceneLoaders{};
		std::shared_ptr<Scene> m_pScene{};
		int m_NextScene{ -1 };
		int m_CurrentScene{};

		bool m_DrawSceneHierarchy{};
	};
}
