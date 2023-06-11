#pragma once

#include "State.h"

namespace digdug
{
	class FygarLogic
	{
	public:
		virtual ~FygarLogic() = default;
		virtual std::unique_ptr<State> Update() = 0;
	};
}