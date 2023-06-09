#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace digdug
{
	class GridTransform;

	class GridMoveCommand final : public that::DataCommand<glm::vec2>
	{
	public:
		GridMoveCommand(GridTransform* pTransform, bool checkWorld = false) : m_pTransform{ pTransform }, m_CheckWorld{ checkWorld } {}
		virtual ~GridMoveCommand() = default;

		virtual void Execute() override;
	private:
		GridTransform* m_pTransform{};
		bool m_CheckWorld{};
	};
}
