#pragma once

#include "Component.h"

#include "Subject.h"

namespace digdug
{
	struct CollisionData
	{
		that::GameObject* collider;
		that::GameObject* other;
	};

	class GridCollider final : public that::Component
	{
	public:
		GridCollider() = default;
		virtual ~GridCollider() = default;

		GridCollider(const GridCollider& other) = delete;
		GridCollider(GridCollider&& other) = delete;
		GridCollider& operator=(const GridCollider& other) = delete;
		GridCollider& operator=(GridCollider&& other) = delete;

		void AddOnCollision(that::Observer<CollisionData>* observer);
		void Hit(GridCollider* pOther);
	private:
		that::Subject<CollisionData> m_Subject{};
	};
}

