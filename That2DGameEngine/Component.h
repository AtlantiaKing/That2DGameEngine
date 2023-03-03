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
		std::shared_ptr<Transform> GetTransform() const;

		bool IsMarkedAsDead() const { return m_IsMarkedDead; };
	protected:
		std::weak_ptr<GameObject> m_pParent{};
	private:
		void SetParent(std::weak_ptr<GameObject> pParent);

		bool m_IsMarkedDead{};
	};

	template<class T>
	inline std::shared_ptr<T> Component::GetComponent() const
	{
		static_assert(std::is_base_of<Component, T>(), "T needs to be derived from the Component class");

		if (m_pParent.expired()) return nullptr;

		return m_pParent.lock()->GetComponent<T>();
	}
}

