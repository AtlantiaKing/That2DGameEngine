#pragma once

#include <memory>

namespace digdug
{
	class PookaState
	{
	public:
		virtual ~PookaState() = default;

		virtual std::unique_ptr<PookaState> Update() = 0;
		virtual void StateEnter() = 0;
		virtual void StateEnd() = 0;
	};
}