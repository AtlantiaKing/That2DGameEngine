#pragma once

#include "Command.h"

namespace that
{
	class GameObject;

	class DebugAxisCommand final : public Command
	{
	public:
		DebugAxisCommand(GameObject* pGameObject);
		virtual ~DebugAxisCommand() = default;

		virtual void Execute() override;
	private:
	};
}