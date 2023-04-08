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
		GridMoveCommand(that::GameObject* pPlayer, float moveSpeed) : m_pPlayer{ pPlayer }, m_MoveSpeed{ moveSpeed } {}
		virtual ~GridMoveCommand() = default;

		virtual void Execute() override;
	private:
		that::GameObject* m_pPlayer{};
		const float m_MoveSpeed{ 16.0f };
	};
}
