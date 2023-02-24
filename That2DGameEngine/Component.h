#pragma once

#include "GameObject.h"
#include <memory>

namespace that
{
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

		template <class T>
		friend std::shared_ptr<T> GameObject::AddComponent();

		template <class T>
		std::shared_ptr<T> GetComponent() const;
	protected:
		std::weak_ptr<GameObject> m_pParent{};
	private:
		void SetParent(std::weak_ptr<GameObject> pParent);
	};

	template<class T>
	inline std::shared_ptr<T> Component::GetComponent() const
	{
		if (m_pParent.expired()) return nullptr;

		return m_pParent.lock()->GetComponent<T>();
	}
}

