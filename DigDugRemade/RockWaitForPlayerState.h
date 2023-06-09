#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent;

	class RockWaitForPlayerState : public EnemyState
	{
	public:
		RockWaitForPlayerState(that::GameObject* pRock);
		virtual ~RockWaitForPlayerState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};
		that::GameObject* m_pPlayer{};

		const float m_GridEpsilonX{ 0.5f };
		const float m_GridEpsilonY{ 1.0f };

		GridComponent* m_pGrid{};
	};
}

