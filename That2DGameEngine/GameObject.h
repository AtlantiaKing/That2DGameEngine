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
		std::shared_ptr<GameObject> GetParent() const;
		std::shared_ptr<GameObject> GetChild(int index) const;
		const std::vector<std::weak_ptr<GameObject>>& GetChildren() const { return m_pChildren; };
		size_t GetChildCount() const { return m_pChildren.size(); };

		void Destroy();
		bool IsMarkedAsDead() const { return m_IsMarkedDead; };

		template <class T>
		std::shared_ptr<T> GetComponent() const;
		template <class T>
		std::vector<std::shared_ptr<T>> GetComponents() const;
		template <class T>
		std::shared_ptr<T> AddComponent();
		template <class T>
		bool RemoveComponent();
		template <class T>
		bool RemoveComponent(std::shared_ptr<T> pComponent);
		template <class T>
		bool HasComponent() const;
		std::shared_ptr<Transform> GetTransform() const { return m_pTransform; };

	private:
		std::weak_ptr<GameObject> m_pParent{};
		std::vector<std::weak_ptr<GameObject>> m_pChildren{};

		std::vector<std::shared_ptr<Component>> m_pComponents{};
		std::shared_ptr<Transform> m_pTransform{};
		std::shared_ptr<TextureRenderer> m_pRenderComponent{};

		bool Destroy(std::shared_ptr<Component> pComponent);

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
	inline std::vector<std::shared_ptr<T>> GameObject::GetComponents() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		std::vector<std::shared_ptr<T>> pComponents{};

		for (const std::shared_ptr<Component>& pComponent : m_pComponents)
		{
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

			if (derivedComponent) pComponents.push_back(derivedComponent);
		}

		return pComponents;
	}

	template<class T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// TODO: If the user adds a component of type Transform (not the gameobject making it automatically)
		//			Log a warning that the user is adding a component that will do nothing and cannot be removed

		// Create a new component
		const auto pComponent{ std::make_shared<T>() };

		// Set the current gameObject as its parent
		pComponent->SetOwner(weak_from_this());

		// If the new component is a RenderComponent, set this component as the render component
		if constexpr (std::is_same<TextureRenderer, T>())
		{
			// TODO: If the user adds a second texture renderer to a gameobject
			//			Log a warning that the previous texture renderer has been discarded

			m_pRenderComponent = std::dynamic_pointer_cast<TextureRenderer>(pComponent);
		}

		// Add this component to the container of components
		m_pComponents.push_back(pComponent);

		// Return the new component
		return pComponent;
	}

	template<class T>
	inline bool GameObject::RemoveComponent()
	{
		static_assert(!std::is_same<Transform, T>(), "Transform component cannot be removed");
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		// For each component on this gameobject
		for (auto it{ begin(m_pComponents) }; it < end(m_pComponents); ++it)
		{
			const auto pComponent{ *it };

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
	inline bool GameObject::RemoveComponent(std::shared_ptr<T> pComponent)
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
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

			// If the cast succeeds, return true
			if (derivedComponent) return true;
		}

		// Return that this gameobject does not have a component of type T
		return false;
	}
}
