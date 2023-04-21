#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include <sstream>

using namespace that;

GameObject* that::Scene::CreateGameObject(const std::string& name)
{
	auto pGameObject{ std::make_unique<GameObject>(this, name) };
	pGameObject->Init();

	GameObject* pGameObjectPtr{ pGameObject.get() };

	m_pObjects.push_back(std::move(pGameObject));

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
	for (auto it{ begin(m_pObjects) }; it < end(m_pObjects); ++it)
	{
		if (it->get() != pGameObject) continue;

		auto pUnique{ std::move(*it) };

		m_pObjects.erase(it);

		return pUnique;
	}

	return nullptr;
}

void that::Scene::Init()
{
	for (auto& object : m_pObjects)
	{
		object->Init();
	}
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
}

void that::Scene::LateUpdate()
{
	for (auto& object : m_pObjects)
	{
		object->LateUpdate();
	}
}

void that::Scene::UpdateCleanup()
{
	// Move all objects marked as dead to the end of the objects container
	const auto& removeIt{ std::remove_if(begin(m_pObjects), end(m_pObjects), [](const auto& pGameObject) { return pGameObject->IsMarkedAsDead(); }) };

	// Call the OnDestroy method of every to-be-destroyed object
	for (auto it{ removeIt }; it < end(m_pObjects); ++it)
	{
		(*it)->OnDestroy();
	}

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
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

void that::Scene::OnGUI()
{
	//RenderScenegraph();

	for (const auto& object : m_pObjects)
	{
		object->OnGUI();
	}
}

that::Scene::~Scene()
{	
	// Call the OnDestroy method of every to-be-destroyed object
	for (const auto& pGameObject : m_pObjects)
	{
		pGameObject->OnDestroy();
	}
}

void that::Scene::RenderScenegraph()
{
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

	for (const auto& go : m_pObjects)
	{
		RenderScenegraphGameObject(go.get());

		ImGui::Spacing();
	}

	ImGui::End();
}

void that::Scene::RenderScenegraphGameObject(GameObject* pGameObject)
{
	const auto& pChildren{ pGameObject->GetChildren() };

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
	for (const auto& pComponent : pGameObject->GetComponents())
	{
		ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, typeid(*pComponent.get()).name());
	}
}
