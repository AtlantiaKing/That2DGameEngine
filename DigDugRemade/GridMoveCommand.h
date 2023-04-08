#pragma once

#include "Command.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridMoveCommand final : public that::Command
	{
	public:
		GridMoveCommand(that::GameObject* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~GridMoveCommand() = default;

		virtual void Execute() override;
	private:
		that::GameObject* m_pPlayer{};
	};
}
