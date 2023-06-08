#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent;

	class RockStaticState : public EnemyState
	{
	public:
		RockStaticState(that::GameObject* pRock, that::GameObject* pPlayer);
		virtual ~RockStaticState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};
		that::GameObject* m_pPlayer{};

		GridComponent* m_pGrid{};
	};
}



