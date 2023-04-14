#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridMoveCommand final : public that::DataCommand<glm::vec2>
	{
	public:
		GridMoveCommand(that::GameObject* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~GridMoveCommand() = default;

		virtual void Execute() override;
	private:
		that::GameObject* m_pPlayer{};
	};
}
