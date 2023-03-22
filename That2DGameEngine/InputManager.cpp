#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>

#include "InputManager.h"
#include "Command.h"
#include "Controller.h"


bool that::InputManager::ProcessInput()
{
	// Check Alt+F4
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		//process event for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (const auto& pController : m_pControllers)
	{
		pController->Update();
	}

	for (const auto& axisCommand : m_pBindedAxisCommands)
	{
		float percentage{ m_pControllers[axisCommand.first.controllerIdx]->GetAxis(axisCommand.first.left, axisCommand.first.x) };

		if (abs(percentage) > 0.0f) axisCommand.second->Execute();
	}

	for (const auto& buttonCommand : m_pBindedButtonCommands)
	{
		bool shouldExecute{};

		switch (buttonCommand.first.inputType)
		{
		case InputType::ONBUTTONDOWN:
			shouldExecute = m_pControllers[buttonCommand.first.controllerIdx]->OnButtonDown(buttonCommand.first.button);
			break;
		case InputType::ONBUTTONUP:
			shouldExecute = m_pControllers[buttonCommand.first.controllerIdx]->OnButtonUp(buttonCommand.first.button);
			break;
		case InputType::ONBUTTON:
			shouldExecute = m_pControllers[buttonCommand.first.controllerIdx]->OnButton(buttonCommand.first.button);
			break;
		}

		if (shouldExecute) buttonCommand.second->Execute();
	}

	return true;
}

void that::InputManager::BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType, std::unique_ptr<Command> pCommand)
{
	if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

	m_pBindedButtonCommands.push_back(std::make_pair(InputKey{ controller, button, inputType }, std::move(pCommand)));
}

void that::InputManager::BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, std::unique_ptr<Command> pCommand)
{
	if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

	m_pBindedAxisCommands.push_back(std::make_pair(InputAxis{ controller, leftJoystick, x }, std::move(pCommand)));
}

float that::InputManager::GetAxis(Command* pCommand)
{
	for (const auto& axisCommand : m_pBindedAxisCommands)
	{
		if (axisCommand.second.get() != pCommand) continue;

		return m_pControllers[axisCommand.first.controllerIdx]->GetAxis(axisCommand.first.left, axisCommand.first.x);
	}

	return 0.0f;
}
