#pragma once

#include "EnemyState.h"
#include "Observer.h"

#include "glm/vec2.hpp"

#include "PhysicsData.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent;

	class RockFallingState : public EnemyState, public that::Observer<that::CollisionData>
	{
	public:
		RockFallingState(that::GameObject* pRock, that::GameObject* pPlayer);
		virtual ~RockFallingState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
		virtual void Notify(const that::CollisionData& collision) override;
	private:
		that::GameObject* m_pRock{};
		that::GameObject* m_pPlayer{};

		glm::ivec2 m_StartPos{};

		GridComponent* m_pGrid{};
	};
}


