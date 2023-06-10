#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace digdug
{
	class DigDug;

	class DisablePumpCommand final : public that::DataCommand<glm::vec2>
	{
	public:
		DisablePumpCommand(DigDug* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~DisablePumpCommand() = default;

		virtual void Execute() override;
	private:
		DigDug* m_pPlayer{};
	};
}
