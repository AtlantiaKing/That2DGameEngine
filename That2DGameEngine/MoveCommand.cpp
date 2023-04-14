#include "MoveCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "Timer.h"

void that::MoveCommand::Execute()
{
	auto& input{ GetData() };
	input.y *= -1.0f;

	Transform* pTransform{ m_pPlayer->GetTransform() };

	glm::vec2 pos{ pTransform->GetLocalPosition() };
	pos += input * m_MoveSpeed * Timer::GetInstance().GetElapsed();

	pTransform->SetLocalPosition(pos);
}
