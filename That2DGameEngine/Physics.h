#pragma once

#include "Singleton.h"
#include <vector>

namespace that
{
	class BoxCollider;

	struct CollisionData
	{
		BoxCollider* collider;
		BoxCollider* other;
	};

	class Physics final : public Singleton<Physics>
	{
	public:
		Physics() = default;
		virtual ~Physics() = default;

		Physics(const Physics& other) = delete;
		Physics(Physics&& other) = delete;
		Physics& operator=(const Physics& other) = delete;
		Physics& operator=(Physics&& other) = delete;

		void AddCollider(BoxCollider* pCollider);
		void RemoveCollider(BoxCollider* pCollider);

		void Update();
	private:
		bool DoOverlap(BoxCollider* pCollider, BoxCollider* pOther) const;

		std::vector<BoxCollider*> m_pColliders{};
	};
}

