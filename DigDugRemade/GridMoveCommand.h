#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace digdug
{
	class DigDug;

	class GridMoveCommand final : public that::DataCommand<glm::vec2>
	{
	public:
		GridMoveCommand(digdug::DigDug* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~GridMoveCommand() = default;

		virtual void Execute() override;
	private:
		digdug::DigDug* m_pPlayer{};
	};
}
