#include "GridMoveCommand.h"

#include "InputManager.h"
#include "GameObject.h"
#include "DigDug.h"
#include "Timer.h"

#include "glm/vec2.hpp"

void digdug::GridMoveCommand::Execute()
{
	auto& input{ GetData() };

	input.y *= -1.0f;

	const glm::ivec2 velocity
	{
		abs(input.x) > 0.0f ? static_cast<int>(input.x / abs(input.x)) : 0,
		abs(input.y) > 0.0f ? static_cast<int>(input.y / abs(input.y)) : 0
	};

	m_pPlayer->Move(velocity);
}
