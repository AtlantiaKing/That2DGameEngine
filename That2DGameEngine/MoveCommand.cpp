#include "MoveCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include "InputManager.h"

void that::MoveCommand::Execute()
{
	const auto& input{ InputManager::GetInstance().GetTwoDirectionalAxis(this) };

	std::cout << "vroom to " << input.x << ", " << input.y << "\n";
}
