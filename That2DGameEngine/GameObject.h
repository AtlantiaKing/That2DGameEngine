#pragma once
#include <vector>
#include <memory>

namespace that
{
	class TextureRenderer;
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void Render() const;

		template <class T>
		std::shared_ptr<T> GetComponent() const;
		template <class T>
		std::shared_ptr<T> AddComponent();
		template <class T>
		bool RemoveComponent();
		template <class T>
		bool HasComponent() const;

	private:
		std::vector<std::shared_ptr<Component>> m_pComponents{};
		std::vector<std::shared_ptr<TextureRenderer>> m_pRenderComponents{};
	};

	template<class T>
	inline std::shared_ptr<T> GameObject::GetComponent() const
	{
		
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
		// Create a new component
		auto pComponent{ std::make_shared<T>() };

		// Set the current gameObject as its parent
		pComponent->SetParent(shared_from_this());

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
		// For each component on this gameobject
		for (auto it{ begin(m_pComponents) }; it < end(m_pComponents); ++it)
		{
			// Try casting the current component to T
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(*it) };

			// If this cast failed, continue to the next component
			if (!derivedComponent) continue;
			
			// Try casting the component to a render component
			//	If this succeeds, find this component in the container of render components and remove it from the container
			std::shared_ptr<TextureRenderer> pAsRenderComponent{ std::dynamic_pointer_cast<TextureRenderer>(derivedComponent) };
			for (auto renderIt{ begin(m_pRenderComponents) }; renderIt < end(m_pRenderComponents); ++renderIt)
			{
				if (renderIt->get() == pAsRenderComponent.get())
				{
					m_pRenderComponents.erase(renderIt);
					break;
				}
			}

			// Remove this component from the container
			m_pComponents.erase(it);

			// Return that the component of type T has been removed
			return true;
		}

		// Return that a component of type T has not been removed
		return false;
	}

	template<class T>
	inline bool GameObject::HasComponent() const
	{
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
