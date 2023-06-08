#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class RockBreakingFreeState : public EnemyState
	{
	public:
		RockBreakingFreeState(that::GameObject* pRock, that::GameObject* pPlayer);
		virtual ~RockBreakingFreeState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};
		that::GameObject* m_pPlayer{};

		float m_CurTime{};
		const float m_TimeUntilFall{ 0.5f };
	};
}


