#pragma once

#include "EnemyState.h"

namespace digdug
{
	class FygarLogic
	{
	public:
		virtual ~FygarLogic() = default;
		virtual std::unique_ptr<EnemyState> Update() = 0;
	};
}