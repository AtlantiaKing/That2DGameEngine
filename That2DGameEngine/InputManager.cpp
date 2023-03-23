#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>

#include "InputManager.h"
#include "Command.h"
#include "Controller.h"
#include "Logger.h"

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

	/*for (const auto& axisCommand : m_pBindedAxisCommands)
	{
		float percentage{ m_pControllers[axisCommand.first.controllerIdx]->GetAxis(axisCommand.first.left, axisCommand.first.x) };

		if (abs(percentage) > 0.0f) axisCommand.second->Execute();
	}*/

	for (const auto& buttonCommand : m_pBindedButtonCommands)
	{
		bool shouldExecute{};

		for (const auto& inputKey : buttonCommand.second)
		{
			switch (inputKey.inputType)
			{
			case InputType::ONBUTTONDOWN:
				shouldExecute = m_pControllers[inputKey.controllerIdx]->OnButtonDown(inputKey.button);
				break;
			case InputType::ONBUTTONUP:
				shouldExecute = m_pControllers[inputKey.controllerIdx]->OnButtonUp(inputKey.button);
				break;
			case InputType::ONBUTTON:
				shouldExecute = m_pControllers[inputKey.controllerIdx]->OnButton(inputKey.button);
				break;
			}

			if (shouldExecute) break;
		}

		if (shouldExecute) buttonCommand.first->Execute();
	}

	return true;
}

void that::InputManager::BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType, std::unique_ptr<Command> pCommand)
{
	if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

	m_pBindedButtonCommands.push_back(std::make_pair(std::move(pCommand), std::vector<InputKey>{ InputKey{ controller, button, inputType } }));
}

void that::InputManager::BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, std::unique_ptr<Command> pCommand)
{
	if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

	m_pBindedAxisCommands.push_back(std::make_pair(std::move(pCommand), std::vector<InputAxis>{ InputAxis{ controller, leftJoystick, x } }));
}

glm::vec2 that::InputManager::GetTwoDirectionalAxis(Command* pCommand)
{
	const auto& pBindedCommand{ std::find_if(begin(m_pBindedButtonCommands), end(m_pBindedButtonCommands), [pCommand](const auto& command)
		{
			return command.first.get() == pCommand;
		})};

	if (pBindedCommand != m_pBindedButtonCommands.end())
	{
		const auto pInputVector{ pBindedCommand->second };

		if (pInputVector.size() != 4)
		{
			Logger::LogWarning("Trying to read a two directional axis from a command that doesn't have 4 inputs binded to it");
			return glm::vec2{ 0.0f, 0.0f };
		}

		glm::vec2 input{};

		for (int i{}; i < pInputVector.size(); ++i)
		{
			const auto& inputKey{ pBindedCommand->second[i] };

			bool hasInput{};

			switch (inputKey.inputType)
			{
			case InputType::ONBUTTONDOWN:
				hasInput = m_pControllers[inputKey.controllerIdx]->OnButtonDown(inputKey.button);
				break;
			case InputType::ONBUTTONUP:
				hasInput = m_pControllers[inputKey.controllerIdx]->OnButtonUp(inputKey.button);
				break;
			case InputType::ONBUTTON:
				hasInput = m_pControllers[inputKey.controllerIdx]->OnButton(inputKey.button);
				break;
			}

			if (!hasInput) continue;

			switch (i)
			{
			case 0:
				input.x += 1.0f;
				break;
			case 1:
				input.x -= 1.0f;
				break;
			case 2:
				input.y += 1.0f;
				break;
			case 3:
				input.y -= 1.0f;
				break;
			}
		}

		return glm::normalize(input);
	}

	// TODO: Handle Axis Commands

	return glm::vec2{ 0.0f, 0.0f };
}
