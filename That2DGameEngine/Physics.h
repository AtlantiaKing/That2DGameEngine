#pragma once

#include "Singleton.h"

#include "PhysicsData.h"

#include <vector>

#include <glm/vec2.hpp>

namespace that
{
	struct CollisionData;
	class BoxCollider;

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

		void ActivateDebugRendering(bool showDebug);
		bool IsShowingDebugRendering() const;

		/// <summary>
		/// Set a margin between colliders before they start collider
		/// </summary>
		void SetCollisionMargin(float margin);

		void Update();

	private:
		bool DoOverlap(CollisionData& collisionData) const;

		std::vector<BoxCollider*> m_pColliders{};

		bool m_ShowDebug{};
		float m_DistanceEpsilon{};
	};
}

