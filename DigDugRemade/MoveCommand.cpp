#include "MoveCommand.h"

#include "Transform.h"

#include "Timer.h"

void digdug::MoveCommand::Execute()
{
	auto& input{ GetData() };

	input.y *= -1.0f;

	const glm::vec2 velocity{ input * m_MoveSpeed * that::Timer::GetInstance().GetElapsed() };

	m_pTransform->Translate(velocity);
}
