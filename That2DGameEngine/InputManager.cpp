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

	for (const auto& bindedCommand : m_pBindedCommands)
	{
		bool shouldExecute{};

		switch (bindedCommand.first.inputType)
		{
		case InputType::ONBUTTONDOWN:
			shouldExecute = m_pControllers[bindedCommand.first.controllerIdx]->OnButtonDown(bindedCommand.first.button);
			break;
		case InputType::ONBUTTONUP:
			shouldExecute = m_pControllers[bindedCommand.first.controllerIdx]->OnButtonUp(bindedCommand.first.button);
			break;
		case InputType::ONBUTTON:
			shouldExecute = m_pControllers[bindedCommand.first.controllerIdx]->OnButton(bindedCommand.first.button);
			break;
		case InputType::AXIS:
			break;
		}

		if (shouldExecute) bindedCommand.second->Execute();
	}

	return true;
}

void that::InputManager::BindCommand(unsigned int controller, unsigned int button, InputType inputType, std::unique_ptr<Command> pCommand)
{
	if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

	m_pBindedCommands.push_back(std::make_pair(InputKey{ controller, button, inputType }, std::move(pCommand)));
}
