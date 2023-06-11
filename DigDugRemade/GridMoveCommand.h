#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace digdug
{
	class GridTransform;
	class GameState;

	class GridMoveCommand final : public that::DataCommand<glm::vec2>
	{
	public:
		GridMoveCommand(GridTransform* pTransform, bool checkWorld = false);
		virtual ~GridMoveCommand() = default;

		virtual void Execute() override;
	private:
		GridTransform* m_pTransform{};
		GameState* m_pState{};
		bool m_CheckWorld{};
	};
}
