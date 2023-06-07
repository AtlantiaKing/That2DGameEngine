#pragma once

#include <memory>

#include "glm/vec2.hpp"

namespace digdug
{
	class DigDugState
	{
	public:
		virtual ~DigDugState() = default;

		virtual std::unique_ptr<DigDugState>  HandleInput(const glm::ivec2& movement, bool pumping, bool pumpHold) = 0;
		virtual std::unique_ptr<DigDugState> Update() = 0;
		virtual void StateEnter() = 0;
		virtual void StateEnd() = 0;
	};
}