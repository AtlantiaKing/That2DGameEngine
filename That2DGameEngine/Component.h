#pragma once

#include "GameObject.h"
#include <memory>

namespace that
{
	class Transform;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void FixedUpdate() {};

		void Destroy();

		template <class T>
		friend std::shared_ptr<T> GameObject::AddComponent();

		template <class T>
		std::shared_ptr<T> GetComponent() const;
		template <class T>
		std::vector<std::shared_ptr<T>> GetComponents() const;
		std::shared_ptr<Transform> GetTransform() const;
		std::shared_ptr<GameObject> GetOwner() const;

		bool IsMarkedAsDead() const { return m_IsMarkedDead; };
	private:
		void SetOwner(std::weak_ptr<GameObject> pParent);

		std::weak_ptr<GameObject> m_pOwner{};
		bool m_IsMarkedDead{};
	};

	template<class T>
	inline std::shared_ptr<T> Component::GetComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		if (m_pOwner.expired()) return nullptr;

		return m_pOwner.lock()->GetComponent<T>();
	}

	template<class T>
	inline std::vector<std::shared_ptr<T>> Component::GetComponents() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		if (m_pOwner.expired()) return nullptr;

		return m_pOwner.lock()->GetComponents<T>();
	}
}

