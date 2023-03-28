#pragma once

#include "Command.h"

namespace that
{
	class GameObject;

	class DebugButtonCommand final : public Command
	{
	public:
		DebugButtonCommand(GameObject*) {};
		virtual ~DebugButtonCommand() = default;

		virtual void Execute() override;
	private:
	};
}
