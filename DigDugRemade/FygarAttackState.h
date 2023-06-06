#pragma once

#include "FygarState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class FygarAttackState : public FygarState
	{
	public:
		FygarAttackState(that::GameObject* pFygar, that::GameObject* pPlayer);
		virtual ~FygarAttackState() = default;

		virtual std::unique_ptr<FygarState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;

	private:
		that::GameObject* m_pFygarObj{};
		that::GameObject* m_pPlayer{};

		that::GameObject* m_pFire{};
	};
}

