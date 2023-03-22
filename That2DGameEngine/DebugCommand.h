#pragma once

#include "Command.h"

namespace that
{
	class DebugCommand final : public Command
	{
	public:
		DebugCommand() = default;
		virtual ~DebugCommand() = default;

		virtual void Execute() override;
	private:
	};
}
