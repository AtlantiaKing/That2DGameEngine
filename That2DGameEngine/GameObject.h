#pragma once

#include "Component.h"
#include <vector>
#include <memory>

namespace that
{
	class TextureRenderer;
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
		std::shared_ptr<GameObject> GetParent() const;
		std::shared_ptr<GameObject> GetChild(int index) const;
		const std::vector<std::weak_ptr<GameObject>>& GetChildren() const { return m_pChildren; };
		size_t GetChildCount() const { return m_pChildren.size(); };

		void Destroy();
		bool IsMarkedAsDead() const { return m_IsMarkedDead; };

		template <class T>
		T* GetComponent() const;
		template <class T>
		std::vector<T*> GetComponents() const;
		template <class T>
		T* AddComponent();
		template <class T>
		bool RemoveComponent();
		template <class T>
		bool RemoveComponent(T* pComponent);
		template <class T>
		bool HasComponent() const;
		Transform* GetTransform() const { return m_pTransform; };

	private:
		bool Destroy(Component* pComponent);

		std::weak_ptr<GameObject> m_pParent{};
		std::vector<std::weak_ptr<GameObject>> m_pChildren{};

		std::vector<std::unique_ptr<Component>> m_pComponents{};
		Transform* m_pTransform{};
		TextureRenderer* m_pRenderComponent{};

		bool m_IsMarkedDead{};
	};

	template<class T>
	inline T* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");
		
		for (const auto& pComponent : m_pComponents)
		{
			T* derivedComponent{ dynamic_cast<T*>(pComponent.get()) };

			if (derivedComponent) return derivedComponent;
		}

		return nullptr;
	}

	template<class T>
	inline std::vector<T*> GameObject::GetComponents() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		std::vector<T*> pComponents{};

		for (const auto& pComponent : m_pComponents)
		{
			T* derivedComponent{ dynamic_cast<T*>(pComponent.get()) };

			if (derivedComponent) pComponents.push_back(derivedComponent);
		}

		return pComponents;
	}

	template<class T>
	inline T* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// TODO: If the user adds a component of type Transform (not the gameobject making it automatically)
		//			Log a warning that the user is adding a component that will do nothing and cannot be removed

		// Create a new component
		auto pComponent{ std::make_unique<T>() };

		// Set the current gameObject as its parent
		pComponent->SetOwner(weak_from_this());

		// Get the actual pointer to the new component
		T* pComponentPtr{ pComponent.get() };

		// If the new component is a RenderComponent, set this component as the render component
		if constexpr (std::is_same<TextureRenderer, T>())
		{
			// TODO: If the user adds a second texture renderer to a gameobject
			//			Log a warning that the previous texture renderer has been discarded

			m_pRenderComponent = static_cast<TextureRenderer*>(pComponentPtr);
		}

		// Add this component to the container of components
		m_pComponents.push_back(std::move(pComponent));

		// Return the new component
		return pComponentPtr;
	}

	template<class T>
	inline bool GameObject::RemoveComponent()
	{
		static_assert(!std::is_same<Transform, T>(), "Transform component cannot be removed");
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// Try getting a component of type T
		const T* pComponent{ GetComponent<T>() };

		// If this component exists, destroy it
		if(pComponent)
		{
			// Mark the component as dead
			Destroy(pComponent);

			// Return that the component of type T has been removed
			return true;
		}

		// Return that a component of type T has not been removed
		return false;
	}

	template<class T>
	inline bool GameObject::RemoveComponent(T* pComponent)
	{
		static_assert(!std::is_same<Transform, T>(), "Transform component cannot be removed");
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		return Destroy(pComponent);
	}

	template<class T>
	inline bool GameObject::HasComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// For each component on this gameobject
		for (const auto& pComponent : m_pComponents)
		{
			// Try casting the current component to T
			T* derivedComponent{ dynamic_cast<T*>(pComponent.get()) };

			// If the cast succeeds, return true
			if (derivedComponent) return true;
		}

		// Return that this gameobject does not have a component of type T
		return false;
	}
}
