#pragma once

#include <memory>
#include "GameObject.h"

namespace that
{
	class GameObject;

	class Component
	{
	public:
		Component(std::weak_ptr<GameObject> pParent);
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void FixedUpdate() {};

		template <class T>
		std::shared_ptr<T> GetComponent();
	protected:
		std::weak_ptr<GameObject> m_pParent{};
	};

	template<class T>
	inline std::shared_ptr<T> Component::GetComponent()
	{
		if (m_pParent.expired()) return nullptr;

		return m_pParent.lock()->GetComponent<T>();
	}
}

