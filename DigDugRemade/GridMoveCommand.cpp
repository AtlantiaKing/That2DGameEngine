#include "GridMoveCommand.h"

#include "InputManager.h"
#include "GameObject.h"
#include "GridTransform.h"
#include "Timer.h"
#include "GameState.h"

#include "glm/vec2.hpp"

digdug::GridMoveCommand::GridMoveCommand(GridTransform* pTransform, bool checkWorld) 
	: m_pTransform{ pTransform }
	, m_CheckWorld{ checkWorld } 
{
	m_pState = pTransform->GetOwner()->GetParent()->GetComponent<GameState>();
}

void digdug::GridMoveCommand::Execute()
{
	if (!m_pState->IsInGame()) return;

	glm::vec2 input{ GetData() };

	input.y *= -1.0f;

	const glm::ivec2 velocity
	{
		abs(input.x) > 0.0f ? static_cast<int>(input.x / abs(input.x)) : 0,
		abs(input.y) > 0.0f ? static_cast<int>(input.y / abs(input.y)) : 0
	};

	m_pTransform->Move(velocity.x, velocity.y, m_CheckWorld);
}
