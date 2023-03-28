#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>

#include "InputManager.h"
#include "Command.h"
#include "Controller.h"
#include "Logger.h"

bool that::InputManager::ProcessInput() 
{
	// Read input from SDL
	if (!ReadSDLInput()) return false;

	// Update all the input states of the controllers
	for (const auto& pController : m_pControllers)
	{
		pController->Update();
	}

	// For each command that should be triggered by analog input
	for (const auto& analogCommand : m_pBindedAnalogCommands)
	{
		// For each button that is binded to this command
		for (const auto& inputAnalog : analogCommand.second)
		{
			// Execute the command if one of its input requirements is met
			const float axisValue{ m_pControllers[inputAnalog.controllerIdx]->GetAxis(inputAnalog.left, inputAnalog.x) };
			if (abs(axisValue) > 0.0f)
			{
				analogCommand.first->Execute();
				break;
			}
		}
	}

	// For each command that should be triggered by digital buttons
	for (const auto& buttonCommand : m_pBindedDigitalCommands)
	{
		// For each button that is binded to this command
		for (const auto& inputKey : buttonCommand.second)
		{
			// Execute the command if one of its input requirements is met
			if (TryInput(inputKey))
			{
				buttonCommand.first->Execute();
				break;
			}
		}
	}

	return true;
}

bool that::InputManager::ReadSDLInput()
{
	m_KeyboardDownInput.clear();
	m_KeyboardUpInput.clear();

	// Check keyboard input
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) return false;

		switch (e.key.type)
		{
		case SDL_KEYUP:
		{
			const auto it{ m_KeyboardInput.find(e.key.keysym.sym) };
			if (it != m_KeyboardInput.end())
			{
				m_KeyboardInput.erase(it);
				m_KeyboardUpInput.insert(e.key.keysym.sym);
			}
			break;
		}
		case SDL_KEYDOWN:
		{
			const auto it{ m_KeyboardInput.find(e.key.keysym.sym) };
			if (it == m_KeyboardInput.end())
			{
				m_KeyboardInput.insert(e.key.keysym.sym);
				m_KeyboardDownInput.insert(e.key.keysym.sym);
			}
			break;
		}
		}

		//process event for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

bool that::InputManager::TryInput(const that::InputManager::InputDigital& inputKey)
{
	if (inputKey.keyboard)
	{
		switch (inputKey.inputType)
		{
		case InputType::ONBUTTONDOWN:
			return m_KeyboardDownInput.find(inputKey.button) != m_KeyboardDownInput.end();
		case InputType::ONBUTTONUP:
			return m_KeyboardUpInput.find(inputKey.button) != m_KeyboardUpInput.end();
		case InputType::ONBUTTON:
			return m_KeyboardInput.find(inputKey.button) != m_KeyboardInput.end();
		}
	}
	else
	{
		switch (inputKey.inputType)
		{
		case InputType::ONBUTTONDOWN:
			return m_pControllers[inputKey.controllerIdx]->OnButtonDown(inputKey.button);
		case InputType::ONBUTTONUP:
			return m_pControllers[inputKey.controllerIdx]->OnButtonUp(inputKey.button);
		case InputType::ONBUTTON:
			return m_pControllers[inputKey.controllerIdx]->OnButton(inputKey.button);
		}
	}

	return false;
}

#pragma region BindCommandFunctions
void that::InputManager::BindDigitalCommand(unsigned int controller, GamepadInput button, InputType inputType, std::unique_ptr<Command> pCommand)
{
	// Add controllers if the controllerIdx is higher then the amount of controllers available
	AddControllersIfNeeded(controller);

	// Create a new command
	m_pBindedDigitalCommands.push_back(std::make_pair(std::move(pCommand), std::vector<InputDigital>{ InputDigital{ false, controller, static_cast<unsigned int>(button), inputType } }));
}

void that::InputManager::BindDigitalCommand(unsigned int key, InputType inputType, std::unique_ptr<Command> pCommand)
{
	// Create a new command
	m_pBindedDigitalCommands.push_back(std::make_pair(std::move(pCommand), std::vector<InputDigital>{ InputDigital{ true, 0, key, inputType } }));
}

void that::InputManager::BindDigital2DAxisCommand(unsigned int controller, const std::vector<GamepadInput>& buttons, std::unique_ptr<Command> pCommand)
{
	// Add controllers if the controllerIdx is higher then the amount of controllers available
	AddControllersIfNeeded(controller);

	// Create a vector of input keys with the given controller and buttons
	std::vector<InputDigital> inputKeys{};
	for (GamepadInput button : buttons)
	{
		inputKeys.push_back(InputDigital{ false, controller, static_cast<unsigned int>(button), InputType::ONBUTTON });
	}

	// Create a new command
	m_pBindedDigitalCommands.push_back(std::make_pair(std::move(pCommand), inputKeys));
}

void that::InputManager::BindDigital2DAxisCommand(const std::vector<unsigned int>& keys, std::unique_ptr<Command> pCommand)
{
	// Create a vector of input keys with the given controller and buttons
	std::vector<InputDigital> inputKeys{};
	for (unsigned int key : keys)
	{
		inputKeys.push_back(InputDigital{ true, 0, key, InputType::ONBUTTON });
	}

	// Create a new command
	m_pBindedDigitalCommands.push_back(std::make_pair(std::move(pCommand), inputKeys));
}

void that::InputManager::BindAnalogCommand(unsigned int controller, bool leftJoystick, bool x, std::unique_ptr<Command> pCommand)
{
	// Add controllers if the controllerIdx is higher then the amount of controllers available
	AddControllersIfNeeded(controller);

	// Create a new command
	m_pBindedAnalogCommands.push_back(std::make_pair(std::move(pCommand), std::vector<InputAnalog>{ InputAnalog{ controller, leftJoystick, x } }));
}

void that::InputManager::BindAnalog2DAxisCommand(unsigned int controller, bool left, std::unique_ptr<Command> pCommand)
{
	// Add controllers if the controllerIdx is higher then the amount of controllers available
	AddControllersIfNeeded(controller);

	// Create a vector of input keys with the given controller and buttons
	std::vector<InputAnalog> inputKeys
	{
		InputAnalog{ controller, left, true },
		InputAnalog{ controller, left, false }
	};

	// Create a new command
	m_pBindedAnalogCommands.push_back(std::make_pair(std::move(pCommand), inputKeys));
}
#pragma endregion

void that::InputManager::Clear()
{
	m_pBindedAnalogCommands.clear();
	m_pBindedDigitalCommands.clear();
}

void that::InputManager::AddControllersIfNeeded(unsigned int controller)
{
	// Add controllers until there is a controller with the id that is needed
	if (controller >= m_pControllers.size())
	{
		const unsigned int nrControllersNeeded{ (controller + 1) - static_cast<unsigned int>(m_pControllers.size()) };
		for (unsigned int i{}; i < nrControllersNeeded; ++i)
		{
			m_pControllers.push_back(std::make_unique<Controller>(static_cast<unsigned int>(m_pControllers.size())));
		}
	}
}

#pragma region GetAxisFunctions
float that::InputManager::GetAxis(Command* pCommand) const
{
	// Get the command/digital input pair with the given command ptr
	const auto& pBindedCommand{ std::find_if(begin(m_pBindedDigitalCommands), end(m_pBindedDigitalCommands), [pCommand](const auto& command)
		{
			return command.first.get() == pCommand;
		}) };

	// If a digital command is found, return the axis of the digital input
	if (pBindedCommand != m_pBindedDigitalCommands.end())
	{
		return GetAxis(pBindedCommand->second);
	}

	// Get the command/digital input pair with the given command ptr
	const auto& pBindedAnalogCommand{ std::find_if(begin(m_pBindedAnalogCommands), end(m_pBindedAnalogCommands), [pCommand](const auto& command)
		{
			return command.first.get() == pCommand;
		}) };


	// If an analog command is found, return the biggest axis binded to this command
	if (pBindedAnalogCommand != m_pBindedAnalogCommands.end())
	{
		float maxInput{};
		for (const InputAnalog& input : pBindedAnalogCommand->second)
		{
			const float curInput{ GetAxis(input) };
			maxInput = abs(curInput) > abs(maxInput) ? curInput : maxInput;
		}
		return maxInput;
	}

	return 0.0f;
}

glm::vec2 that::InputManager::GetTwoDirectionalAxis(Command* pCommand) const
{
	// Get the command/digital input pair with the given command ptr
	const auto& pBindedCommand{ std::find_if(begin(m_pBindedDigitalCommands), end(m_pBindedDigitalCommands), [pCommand](const auto& command)
		{
			return command.first.get() == pCommand;
		})};

	// If a digital command is found, return the 2D axis of the digital input
	if (pBindedCommand != m_pBindedDigitalCommands.end())
	{
		return GetTwoDirectionalAxis(pBindedCommand->second);
	}

	// Get the command/digital input pair with the given command ptr
	const auto& pBindedAnalogCommand{ std::find_if(begin(m_pBindedAnalogCommands), end(m_pBindedAnalogCommands), [pCommand](const auto& command)
		{
			return command.first.get() == pCommand;
		}) };


	// If an analog command is found, return the 2D axis of the analog input
	if (pBindedAnalogCommand != m_pBindedAnalogCommands.end())
	{
		return GetTwoDirectionalAxis(pBindedAnalogCommand->second);
	}

	return glm::vec2{};
}

glm::vec2 that::InputManager::GetTwoDirectionalAxis(const std::vector<InputDigital>& inputVector) const
{
	// If there are not exact 4 inputs binded to this command, log a warning and return nothing
	if (inputVector.size() != 4)
	{
		Logger::LogWarning("Trying to read a two directional digital axis from a command that doesn't have 4 inputs binded to it");
		return glm::vec2{ 0.0f, 0.0f };
	}

	glm::vec2 input{};

	// For each button
	for (int i{}; i < static_cast<int>(inputVector.size()); ++i)
	{
		const auto& inputKey{ inputVector[i] };

		// Check if input requirement is met
		bool hasInput{};
		if (inputKey.keyboard)
		{
			if (m_KeyboardInput.find(inputKey.button) != m_KeyboardInput.end()) hasInput = true;
		}
		else
		{
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
		}

		// If this button's input is not triggered, continue to the next button
		if (!hasInput) continue;

		// Calculate the input vector for this button
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

	// Return the normalized 2D axis
	return glm::dot(input,input) > 0.0f ? glm::normalize(input) : input;
}

glm::vec2 that::InputManager::GetTwoDirectionalAxis(const std::vector<InputAnalog>& inputVector) const
{
	// If there are not exact 4 inputs binded to this command, log a warning and return nothing
	if (inputVector.size() != 2)
	{
		Logger::LogWarning("Trying to read a two directional analog axis from a command that doesn't have 2 inputs binded to it");
		return glm::vec2{ 0.0f, 0.0f };
	}

	// Retrieve the two input data
	const auto& horizontalInput{ inputVector[0] };
	const auto& verticalInput{ inputVector[1] };

	// Calculate the input vector for this button
	glm::vec2 input
	{
		m_pControllers[horizontalInput.controllerIdx]->GetAxis(horizontalInput.left, horizontalInput.x),
		m_pControllers[verticalInput.controllerIdx]->GetAxis(verticalInput.left, verticalInput.x)
	};

	return input;
}

float that::InputManager::GetAxis(const std::vector<InputDigital>& inputVector) const
{
	float input{};

	// For each button
	for (int i{}; i < static_cast<int>(inputVector.size()); ++i)
	{
		const auto& inputKey{ inputVector[i] };

		// Check if input requirement is met
		bool hasInput{};
		if (inputKey.keyboard)
		{
			if (m_KeyboardInput.find(inputKey.button) != m_KeyboardInput.end()) hasInput = true;
		}
		else
		{
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
		}

		// If this button's input is not triggered, continue to the next button
		if (!hasInput) continue;

		// Calculate the input value for this button
		switch (i)
		{
		case 0:
			input += 1.0f;
			break;
		case 1:
			input -= 1.0f;
			break;
		}
	}

	// Return the axis
	return input;
}

float that::InputManager::GetAxis(const InputAnalog& input) const
{
	return m_pControllers[input.controllerIdx]->GetAxis(input.left, input.x);
}
#pragma endregion