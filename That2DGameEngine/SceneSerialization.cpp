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
	// Get all the components on this gameobject
	const auto& pComponents{ pGameObject->GetComponents() };

	// Write the amount of components to the file
	const size_t nrComponents{ pComponents.size() };
	file.write(reinterpret_cast<const char*>(&nrComponents), sizeof(size_t));

	// Serialize each component
	for (auto pComponent : pComponents)
	{
		SerializeComponent(pComponent, file);
	}

	// Get all the children on this gameobject
	const auto& pChildren{ pGameObject->GetChildren() };

	// Write the amount of children to the file
	const size_t nrChildren{ pChildren.size() };
	file.write(reinterpret_cast<const char*>(&nrChildren), sizeof(size_t));

	// Serialize each child
	for (auto pChild : pChildren)
	{
		SerializeGameObject(pChild, file);
	}
}

void that::reflection::SceneSerialization::DeserializeGameObject(that::Scene* pScene, std::ifstream& file)
{
	that::GameObject* pCurObj{};

	// Read how many components are stored
	size_t nrComponents{};
	file.read(reinterpret_cast<char*>(&nrComponents), sizeof(size_t));

	while (nrComponents > 0)
	{
		// read the hash of the current component
		size_t hash{}, size{};
		file.read(reinterpret_cast<char*>(&hash), sizeof(size_t));

		// Make sure the hash is not 0 (0 = wrong read)
		if (hash == 0) break;

		// If no gameobject has been created yet, create a new gameobject as a child or in the scene
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

		// Read the size of the component
		file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

		// Get the component that has to be added
		const SerializableComponent& type{ Reflection::GetType(hash) };

		// Add the component to the gameobject
		Component* pComponent{ type.Clone(pCurObj) };

		// Deserialize all the component variables
		for (const auto& var : type.variables)
		{
			file.read(reinterpret_cast<char*>(pComponent) + var.offset, var.size);
		}

		--nrComponents;
	}

	// Read the amount of children this gameobject has
	size_t nrChildren{};
	file.read(reinterpret_cast<char*>(&nrChildren), sizeof(size_t));

	// If there are children, add the current object on the stack and deserialize its children
	if (nrChildren > 0)
	{
		m_ChildrenStack.push(pCurObj);

		for (size_t i{ 0 }; i < nrChildren; ++i)
		{
			DeserializeGameObject(pScene, file);
		}

		m_ChildrenStack.pop();
	}
}

void that::reflection::SceneSerialization::SerializeComponent(that::Component* pComponent, std::ofstream& file)
{
	// Get the info about the component that has to be serialized
	const SerializableComponent& type{ Reflection::GetType(pComponent->GetHash()) };

	// Write the hash and size of the component to the file
	file.write(reinterpret_cast<const char*>(&type.hash), sizeof(size_t));
	file.write(reinterpret_cast<const char*>(&type.size), sizeof(size_t));

	// Write the binary info of each variable of the component to the file
	for (const auto& var : type.variables)
	{
		file.write(reinterpret_cast<const char*>(pComponent) + var.offset, var.size);
	}
}
