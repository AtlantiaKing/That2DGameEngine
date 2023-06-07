#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarAttackState : public State
	{
	public:
		FygarAttackState(that::GameObject* pFygar, that::GameObject* pPlayer);
		virtual ~FygarAttackState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		that::GameObject* m_pFygarObj{};
		that::GameObject* m_pPlayer{};

		that::GameObject* m_pFire{};
	};
}

