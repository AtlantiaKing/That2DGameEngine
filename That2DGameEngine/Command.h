#pragma once

namespace that
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}

