#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include <sstream>

using namespace that;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

GameObject* that::Scene::CreateGameObject(const std::string& name)
{
	auto pGameObject{ std::make_unique<GameObject>(this, name) };
	pGameObject->Init();

	GameObject* pGameObjectPtr{ pGameObject.get() };

	m_objects.push_back(std::move(pGameObject));

	return pGameObjectPtr;
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void that::Scene::Add(std::unique_ptr<GameObject> pGameObject)
{
	m_objects.push_back(std::move(pGameObject));
}

std::unique_ptr<GameObject> that::Scene::GetUnique(GameObject* pGameObject)
{
	for (auto it{ begin(m_objects) }; it < end(m_objects); ++it)
	{
		if (it->get() != pGameObject) continue;

		auto pUnique{ std::move(*it) };

		m_objects.erase(it);

		return pUnique;
	}

	return nullptr;
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void that::Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void that::Scene::UpdateCleanup()
{
	// Remove gameobjects from their container if they are marked as dead
	m_objects.erase(std::remove_if(begin(m_objects), end(m_objects), [](const auto& pGameObject)
		{
			return pGameObject->IsMarkedAsDead();
		}), end(m_objects));

	// Remove all components that are marked as dead
	for (auto& object : m_objects)
	{
		object->UpdateCleanup();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void that::Scene::OnGUI()
{
	RenderScenegraph();

	for (const auto& object : m_objects)
	{
		object->OnGUI();
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

	for (const auto& go : m_objects)
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
