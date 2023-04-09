#pragma once

#include "Command.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class ShootPumpCommand : public that::Command
	{
	public:
		ShootPumpCommand(that::GameObject* pPump) : m_pPump{ pPump } {}
		virtual ~ShootPumpCommand() = default;

		virtual void Execute() override;
	private:
		that::GameObject* m_pPump{};
	};
}
