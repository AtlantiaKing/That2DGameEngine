#pragma once

#include "Command.h"

namespace digdug
{
	class DigDug;

	class ShootPumpCommand : public that::Command
	{
	public:
		ShootPumpCommand(digdug::DigDug* pPlayer) : m_pPlayer{ pPlayer } {}
		virtual ~ShootPumpCommand() = default;

		virtual void Execute() override;
	private:
		digdug::DigDug* m_pPlayer{};
	};
}
