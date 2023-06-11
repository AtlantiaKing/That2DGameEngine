#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include <sstream>
#include <algorithm>

using namespace that;

that::Scene::~Scene()
{
	// Call the OnDestroy method of every object
	for (const auto& pGameObject : m_pObjects)
	{
		pGameObject->OnDestroy();
	}
}

GameObject* that::Scene::CreateGameObject(const std::string& name)
{
	// Create a new gameobject
	auto pGameObject{ std::make_unique<GameObject>(this, name) };

	// Get the raw pointer to the gameobject
	GameObject* pGameObjectPtr{ pGameObject.get() };

	// Add the new object to the list
	m_pObjectsToAdd.push_back(std::move(pGameObject));

	// Return the raw pointer to the new gameobject
	return pGameObjectPtr;
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}

void that::Scene::Add(std::unique_ptr<GameObject> pGameObject)
{
	m_pObjects.push_back(std::move(pGameObject));
}

std::unique_ptr<GameObject> that::Scene::GetUnique(GameObject* pGameObject)
{
	// For each object
	for (auto it{ begin(m_pObjects) }; it < end(m_pObjects); ++it)
	{
		// If the pointers don't match, continue to the next object
		if (it->get() != pGameObject) continue;

		// Retrieve the unique ptr to this gameobject
		auto pUnique{ std::move(*it) };

		// Erase the empty unique ptr from the list
		m_pObjects.erase(it);

		// Return the unique ptr
		return pUnique;
	}

	// This gameobject is not a object that is parented to the scene so return nothing
	return nullptr;
}

void that::Scene::OnFrameStart()
{
	// Add any new children that have been added last frame
	if (!m_pObjectsToAdd.empty())
	{
		std::vector<std::unique_ptr<GameObject>> pNewObjects{ std::move(m_pObjectsToAdd) };

		for (auto& pChild : pNewObjects)
		{
			pChild->Init();
			m_pObjects.push_back(std::move(pChild));
		}
	}

	// Call OnFrameStart on every child
	for (auto& object : m_pObjects)
	{
		object->OnFrameStart();
	}
}

void Scene::Update()
{
	// Call Update on every child
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
}

void that::Scene::LateUpdate()
{
	// Call LateUpdate on every child
	for (auto& object : m_pObjects)
	{
		object->LateUpdate();
	}
}

void that::Scene::UpdateCleanup()

{	// Call the OnDestroy method of every to-be-destroyed object
	std::for_each(m_pObjects.begin(), m_pObjects.end(), [&](const auto& pGameObject) 
	{
		if (pGameObject->IsMarkedAsDead()) 
		{
			pGameObject->OnDestroy();
		}
	});

	// Move all objects marked as dead to the end of the objects container
	const auto& removeIt{ std::remove_if(begin(m_pObjects), end(m_pObjects), [](const auto& pGameObject) { return pGameObject->IsMarkedAsDead(); }) };

	// Remove gameobjects from their container if they are marked as dead
	m_pObjects.erase(removeIt, end(m_pObjects));

	// Remove all components that are marked as dead
	for (auto& object : m_pObjects)
	{
		object->UpdateCleanup();
	}
}

void Scene::Render() const
{
	// Call Render on every child
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

void that::Scene::OnGUI()
{
	// Render the scenegraph
	//RenderScenegraph();

	// Call OnGui on every child
	for (const auto& object : m_pObjects)
	{
		object->OnGUI();
	}
}

void that::Scene::RenderScenegraph()
{
	// Set data for the new window
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 20, main_viewport->WorkPos.y + 60));
	ImGui::SetNextWindowSize(ImVec2(280, 400));

	// Main body of the Demo window starts here.
	if (!ImGui::Begin("Scenegraph"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	// Draw each object in the scene
	for (const auto& go : m_pObjects)
	{
		RenderScenegraphGameObject(go.get());

		ImGui::Spacing();
	}

	// End this ImGui call
	ImGui::End();
}

void that::Scene::RenderScenegraphGameObject(GameObject* pGameObject)
{
	const auto& pChildren{ pGameObject->GetChildren() };

	// Draw every component and child of this gameobject
	if (ImGui::TreeNode(pGameObject->GetName().c_str()))
	{
		if (ImGui::TreeNode("Components:"))
		{
			RenderScenegraphComponents(pGameObject);

			ImGui::TreePop();
		}

		if (!pChildren.empty())
		{
			ImGui::Spacing();

			for (const auto& go : pChildren)
			{
				RenderScenegraphGameObject(go);
			}
		}

		ImGui::TreePop();
	}
}

void that::Scene::RenderScenegraphComponents(GameObject* pGameObject)
{
	for (auto pComponent : pGameObject->GetComponents())
	{
		ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, typeid(pComponent).name());
	}
}
