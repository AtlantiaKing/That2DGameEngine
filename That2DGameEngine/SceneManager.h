#pragma once
#include "Singleton.h"

#include <vector>
#include <memory>
#include <functional>
#include "Scene.h"

namespace that
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void LoadScene(unsigned int index);
		void AddScene(const std::function<void(Scene&)>& sceneLoader);

		void Update();
		void LateUpdate();
		void Render();
		void OnGUI();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::function<void(Scene&)>> m_SceneLoaders{};
		std::shared_ptr<Scene> m_pScene{};
	};
}
