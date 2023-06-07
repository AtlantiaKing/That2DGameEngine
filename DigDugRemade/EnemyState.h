#pragma once

#include <memory>

namespace digdug
{
	class EnemyState
	{
	public:
		virtual ~EnemyState() = default;

		virtual std::unique_ptr<EnemyState> Update() = 0;
		virtual void StateEnter() = 0;
		virtual void StateEnd() = 0;
	};
}