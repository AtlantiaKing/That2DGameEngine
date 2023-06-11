#pragma once
#include "GameObject.h"

namespace that
{
	class Scene final
	{
	public:
		GameObject* CreateGameObject(const std::string& name);
		void RemoveAll();

		void Add(std::unique_ptr<GameObject> pGameObject);
		std::unique_ptr<GameObject> GetUnique(GameObject* pGameObject);

		void OnFrameStart();
		void Update();
		void LateUpdate();
		void UpdateCleanup();
		void Render() const;
		void OnGUI(bool renderHierarchy);

		Scene() = default;
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		void RenderScenegraph();
		void RenderScenegraphGameObject(GameObject* pGameObject);
		void RenderScenegraphComponents(GameObject* pGameObject);

		std::vector<std::unique_ptr<GameObject>> m_pObjects{};
		std::vector<std::unique_ptr<GameObject>> m_pObjectsToAdd{};
	};

}
