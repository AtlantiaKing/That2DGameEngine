#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace that
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		GameObject* CreateGameObject();
		void RemoveAll();

		void Add(std::unique_ptr<GameObject> pGameObject);
		std::unique_ptr<GameObject> GetUnique(GameObject* pGameObject);

		void Update();
		void LateUpdate();
		void UpdateCleanup();
		void Render() const;
		void OnGUI();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector<std::unique_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
