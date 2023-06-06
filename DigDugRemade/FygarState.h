#pragma once

#include <memory>

namespace digdug
{
	class FygarState
	{
	public:
		virtual ~FygarState() = default;

		virtual std::unique_ptr<FygarState> Update() = 0;
		virtual void StateEnter() = 0;
		virtual void StateEnd() = 0;
	};
}

