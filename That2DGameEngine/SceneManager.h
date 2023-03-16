#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace that
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void LateUpdate();
		void Render();
		void OnGUI();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
