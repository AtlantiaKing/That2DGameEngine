#include "GridMoveCommand.h"

#include "InputManager.h"
#include "GameObject.h"
#include "GridTransform.h"
#include "Timer.h"

#include "glm/vec2.hpp"

void digdug::GridMoveCommand::Execute()
{
	glm::vec2 input{ that::InputManager::GetInstance().GetTwoDirectionalAxis(this) };
	input.y *= -1.0f;

	GridTransform* pTransform{ m_pPlayer->GetComponent<GridTransform>() };

	const glm::ivec2 velocity
	{
		abs(input.x) > 0.0f ? static_cast<int>(input.x / abs(input.x)) : 0,
		abs(input.y) > 0.0f ? static_cast<int>(input.y / abs(input.y)) : 0
	};

	pTransform->Move(velocity.x, velocity.y);
}
