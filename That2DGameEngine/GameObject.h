#pragma once

#include <vector>
#include <memory>

namespace that
{
	class TextureRenderer;
	class Component;
	class Transform;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Init();
		void Update();
		void LateUpdate();
		void UpdateCleanup();
		void Render() const;

		void SetParent(std::shared_ptr<GameObject> pParent);

		void Destroy();
		bool IsMarkedAsDead() const { return m_IsMarkedDead; };

		template <class T>
		std::shared_ptr<T> GetComponent() const;
		template <class T>
		std::shared_ptr<T> AddComponent();
		template <class T>
		bool RemoveComponent();
		template <class T>
		bool HasComponent() const;
		std::shared_ptr<Transform> GetTransform() const { return m_pTransform; };

	private:
		std::weak_ptr<GameObject> m_pParent{};
		std::vector<std::weak_ptr<GameObject>> m_pChildren{};

		std::shared_ptr<Transform> m_pTransform{};
		std::vector<std::shared_ptr<Component>> m_pComponents{};
		std::vector<std::shared_ptr<TextureRenderer>> m_pRenderComponents{};

		void Destroy(std::shared_ptr<Component> pComponent);

		bool m_IsMarkedDead{};
	};

	template<class T>
	inline std::shared_ptr<T> GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");
		
		for (const std::shared_ptr<Component>& pComponent : m_pComponents)
		{
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

			if (derivedComponent) return derivedComponent;
		}

		return nullptr;
	}

	template<class T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// Create a new component
		auto pComponent{ std::make_shared<T>() };

		// Set the current gameObject as its parent
		pComponent->SetParent(weak_from_this());

		// Try casting the new component to a RenderComponent, if this succeeds, add this component to the container of render components
		std::shared_ptr<TextureRenderer> pAsRenderComponent{ std::dynamic_pointer_cast<TextureRenderer>(pComponent) };
		if (pAsRenderComponent)
		{
			m_pRenderComponents.push_back(pAsRenderComponent);
		}

		// Add this component to the container of components
		m_pComponents.push_back(pComponent);

		// Return the new component
		return pComponent;
	}

	template<class T>
	inline bool GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// For each component on this gameobject
		for (auto it{ begin(m_pComponents) }; it < end(m_pComponents); ++it)
		{
			auto pComponent{ *it };

			// Try casting the current component to T
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

			// If this cast failed, continue to the next component
			if (!derivedComponent) continue;
			
			// Mark the component as dead
			Destroy(pComponent);

			// Return that the component of type T has been removed
			return true;
		}

		// Return that a component of type T has not been removed
		return false;
	}

	template<class T>
	inline bool GameObject::HasComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// For each component on this gameobject
		for (const auto& pComponent : m_pComponents)
		{
			// Try casting the current component to T
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

			// If the cast succeeds, return true
			if (derivedComponent) return true;
		}

		// Return that this gameobject does not have a component of type T
		return false;
	}
}
