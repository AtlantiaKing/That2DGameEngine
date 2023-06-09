#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarAttackState : public EnemyState
	{
	public:
		FygarAttackState(that::GameObject* pFygar);
		virtual ~FygarAttackState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		that::GameObject* m_pFygarObj{};

		that::GameObject* m_pFire{};
	};
}

