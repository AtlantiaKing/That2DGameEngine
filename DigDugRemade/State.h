#pragma once

#include <memory>

namespace digdug
{
	class State
	{
	public:
		virtual ~State() = default;

		virtual std::unique_ptr<State> Update() = 0;
		virtual void StateEnter() = 0;
		virtual void StateEnd() = 0;
	};
}