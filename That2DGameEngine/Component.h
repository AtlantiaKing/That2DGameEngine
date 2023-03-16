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
		virtual void OnGUI() {};

		void Destroy();

		friend GameObject;

		Transform* GetTransform() const;
		GameObject* GetOwner() const { return m_pOwner; };

		bool IsMarkedAsDead() const { return m_IsMarkedDead; };
	private:
		void SetOwner(GameObject* pParent);

		GameObject* m_pOwner{};
		bool m_IsMarkedDead{};
	};
}
