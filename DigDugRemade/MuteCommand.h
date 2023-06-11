#pragma once

#include "Command.h"

namespace digdug
{
	class MuteCommand final : public that::Command
	{
	public:
		MuteCommand() = default;
		virtual ~MuteCommand() = default;

		virtual void Execute() override;

	private:
		bool m_IsMuted{};
	};
}

