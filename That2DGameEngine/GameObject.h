#pragma once
#include <vector>
#include <memory>

namespace that
{
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		virtual void Update();
		virtual void Render() const;

		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <class T>
		std::shared_ptr<T> GetComponent();
		template <class T>
		std::shared_ptr<T> AddComponent();
		template <class T>
		bool RemoveComponent();
	private:
		std::vector<std::shared_ptr<Component>> m_pComponents{};
	};

	template<class T>
	inline std::shared_ptr<T> GameObject::GetComponent()
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
		auto pComponent{ std::make_shared<T>(shared_from_this()) };

		m_pComponents.push_back(pComponent);

		return pComponent;
	}

	template<class T>
	inline bool GameObject::RemoveComponent()
	{
		for (auto it{ begin(m_pComponents) }; it < end(m_pComponents); ++it)
		{
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(*it) };

			if (derivedComponent)
			{
				m_pComponents.erase(it);
				return true;
			};
		}

		return false;
	}
}
