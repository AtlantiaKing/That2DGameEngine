#pragma once

#include "Command.h"

namespace that
{
	class GameObject;

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~MoveCommand() = default;

		virtual void Execute() override;
	private:
		GameObject* m_pPlayer{};
	};
}

