#pragma once

#include <iosfwd>
#include <stack>

namespace that
{
	class Scene;
	class GameObject;
	class Component;
}

namespace that::reflection
{
	class SceneSerialization final
	{
	public:
		static void SerializeScene(that::Scene* pScene);

		static void DeserializeScene(that::Scene* pScene);

	private:
		static void SerializeGameObject(that::GameObject* pGameObject, std::ofstream& file);

		static void DeserializeGameObject(that::Scene* pScene, std::ifstream& file);

		static void SerializeComponent(that::Component* pComponent, std::ofstream& file);

		inline static std::stack<that::GameObject*> m_ChildrenStack{};
	};
}