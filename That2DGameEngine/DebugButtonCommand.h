#pragma once

#include "Command.h"

namespace that
{
	class DebugButtonCommand final : public Command
	{
	public:
		DebugButtonCommand() = default;
		virtual ~DebugButtonCommand() = default;

		virtual void Execute() override;
	private:
	};
}
