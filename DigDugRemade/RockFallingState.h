#pragma once

#include "State.h"
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

	class RockFallingState : public State, public that::Observer<that::CollisionData>
	{
	public:
		RockFallingState(that::GameObject* pRock);
		virtual ~RockFallingState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
		virtual void Notify(const that::CollisionData& collision) override;
	private:
		int CalculateScore();

		that::GameObject* m_pRock{};

		glm::ivec2 m_StartPos{};

		int m_EnemiesKilled{};

		GridComponent* m_pGrid{};
	};
}


