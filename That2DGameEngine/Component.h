#pragma once

#include <memory>

namespace that
{
	class GameObject;
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

		friend GameObject;

		Transform* GetTransform() const;
		std::shared_ptr<GameObject> GetOwner() const;

		bool IsMarkedAsDead() const { return m_IsMarkedDead; };
	private:
		void SetOwner(std::weak_ptr<GameObject> pParent);

		std::weak_ptr<GameObject> m_pOwner{};
		bool m_IsMarkedDead{};
	};
}
