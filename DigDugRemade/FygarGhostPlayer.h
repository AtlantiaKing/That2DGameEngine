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
	class FygarGhostPlayer final : public FygarLogic
	{
	public:
		FygarGhostPlayer(that::GameObject* pFygar);
		virtual ~FygarGhostPlayer();

		virtual std::unique_ptr<State> Update() override;

	private:
		that::GameObject* m_pFygarObj{};

		that::Command* m_pMoveCommand{};

		const float m_MoveSpeed{ 25.0f };
	};
}
