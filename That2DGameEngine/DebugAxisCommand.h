#pragma once

#include "DataCommand.h"

namespace that
{
	class GameObject;

	class DebugAxisCommand final : public DataCommand<float>
	{
	public:
		DebugAxisCommand(GameObject* pGameObject);
		virtual ~DebugAxisCommand() = default;

		virtual void Execute() override;
	private:
	};
}