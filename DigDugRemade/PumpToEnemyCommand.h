#pragma once

#include "Command.h"

namespace digdug
{
	class DigDug;

	class PumpToEnemyCommand : public that::Command
	{
	public:
		PumpToEnemyCommand(digdug::DigDug* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~PumpToEnemyCommand() = default;

		virtual void Execute() override;
	private:
		digdug::DigDug* m_pPlayer{};
	};
}
