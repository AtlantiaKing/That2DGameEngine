#include "MoveCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "Time.h"

void that::MoveCommand::Execute()
{
	glm::vec2 input{ InputManager::GetInstance().GetTwoDirectionalAxis(this) };
	input.y *= -1.0f;

	Transform* pTransform{ m_pPlayer->GetTransform() };

	glm::vec2 pos{ pTransform->GetLocalPosition() };
	pos += input * m_MoveSpeed * Time::GetInstance().GetElapsed();

	pTransform->SetLocalPosition(pos);
}