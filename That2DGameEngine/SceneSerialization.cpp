#include "SceneSerialization.h"

#include "Scene.h"
#include "GameObject.h"
#include "Component.h"

#include "Reflection.h"

#include <fstream>

void that::reflection::SceneSerialization::SerializeScene(that::Scene* pScene)
{
	std::ofstream file{};
	file.open("serializeddata.that", std::ios::binary);

	for (auto go : pScene->GetObjects())
	{
		SerializeGameObject(go, file);
	}
}

void that::reflection::SceneSerialization::DeserializeScene(that::Scene* pScene)
{
	std::ifstream file{};
	file.open("serializeddata.that", std::ios::binary);

	while (file.good())
	{
		DeserializeGameObject(pScene, file);
	}
}

void that::reflection::SceneSerialization::SerializeGameObject(that::GameObject* pGameObject, std::ofstream& file)
{
	const auto& pComponents{ pGameObject->GetComponents() };

	const size_t nrComponents{ pComponents.size() };
	file.write(reinterpret_cast<const char*>(&nrComponents), sizeof(size_t));

	for (auto pComponent : pComponents)
	{
		SerializeComponent(pComponent, file);
	}

	const auto& pChildren{ pGameObject->GetChildren() };

	const size_t nrChildren{ pChildren.size() };
	file.write(reinterpret_cast<const char*>(&nrChildren), sizeof(size_t));

	for (auto pChild : pChildren)
	{
		SerializeGameObject(pChild, file);
	}
}

void that::reflection::SceneSerialization::DeserializeGameObject(that::Scene* pScene, std::ifstream& file)
{
	that::GameObject* pCurObj{};

	size_t nrComponents{};
	file.read(reinterpret_cast<char*>(&nrComponents), sizeof(size_t));

	while (nrComponents > 0)
	{
		size_t hash{}, size{};
		file.read(reinterpret_cast<char*>(&hash), sizeof(size_t));

		if (hash == 0) break;

		if (pCurObj == nullptr)
		{
			if (m_ChildrenStack.empty())
			{
				pCurObj = pScene->CreateGameObject("GameObject");
			}
			else
			{
				pCurObj = m_ChildrenStack.top()->CreateGameObject("GameObject");
			}
		}

		file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

		const SerializableComponent& type{ Reflection::GetType(hash) };

		Component* pComponent{ type.Clone(pCurObj) };
		for (const auto& var : type.variables)
		{
			file.read(reinterpret_cast<char*>(pComponent) + var.offset, var.size);
		}

		--nrComponents;
	}

	size_t nrChildren{};
	file.read(reinterpret_cast<char*>(&nrChildren), sizeof(size_t));

	if (nrChildren > 0) m_ChildrenStack.push(pCurObj);

	for (size_t i{ 0 }; i < nrChildren; ++i)
	{
		DeserializeGameObject(pScene, file);
	}

	if (nrChildren > 0) m_ChildrenStack.pop();
}

void that::reflection::SceneSerialization::SerializeComponent(that::Component* pComponent, std::ofstream& file)
{
	const SerializableComponent& type{ Reflection::GetType(pComponent->GetHash()) };

	file.write(reinterpret_cast<const char*>(&type.hash), sizeof(size_t));
	file.write(reinterpret_cast<const char*>(&type.size), sizeof(size_t));

	for (const auto& var : type.variables)
	{
		file.write(reinterpret_cast<const char*>(pComponent) + var.offset, var.size);
	}
}
