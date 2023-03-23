#pragma once

#include "Command.h"

namespace that
{
	class GameObject;

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* pPlayer, float moveSpeed) : m_pPlayer{ pPlayer }, m_MoveSpeed{ moveSpeed } {}
		virtual ~MoveCommand() = default;

		virtual void Execute() override;
	private:
		GameObject* m_pPlayer{};
		const float m_MoveSpeed{ 10.0f };
	};
}

