#pragma once

#include "FygarLogic.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
	class Command;
}

namespace digdug
{
	class FygarRoamingPlayer final : public FygarLogic
	{
	public:
		FygarRoamingPlayer(that::GameObject* pFygar);
		virtual ~FygarRoamingPlayer();

		virtual std::unique_ptr<EnemyState> Update() override;

	private:
		that::GameObject* m_pFygarObj{};

		that::Command* m_pMoveCommand{};
		that::Command* m_pFireCommand{};
		that::Command* m_pGhostCommand{};

		bool m_ActivateFire{};
		bool m_Ghost{};
	};
}
