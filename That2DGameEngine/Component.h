#pragma once

#include <memory>

class GameObject;

namespace that
{
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
	protected:
		std::weak_ptr<GameObject> m_pParent{};
	};
}

