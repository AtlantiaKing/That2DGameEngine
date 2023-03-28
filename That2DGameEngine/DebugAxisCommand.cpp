#include "DebugAxisCommand.h"
#include <iostream>
#include "InputManager.h"

void that::DebugAxisCommand::Execute()
{
	std::cout << "Axis value detected " << InputManager::GetInstance().GetAxis(this) << "\n";
}

that::DebugAxisCommand::DebugAxisCommand(GameObject*)
{
}
