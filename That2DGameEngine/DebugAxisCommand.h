#pragma once

#include "Command.h"

namespace that
{
	class DebugAxisCommand final : public Command
	{
	public:
		DebugAxisCommand() = default;
		virtual ~DebugAxisCommand() = default;

		virtual void Execute() override;
	private:
	};
}