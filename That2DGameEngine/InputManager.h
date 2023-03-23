#pragma once
#include "Singleton.h"

#include <memory>
#include <vector>

#include "Command.h"
#include "Controller.h"
#include <glm/glm.hpp>

namespace that
{
	class GameObject;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class InputType
		{
			ONBUTTONDOWN,
			ONBUTTONUP,
			ONBUTTON
		};

		bool ProcessInput();

		template <class T>
		void BindDigitalCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject);
		void BindDigitalCommand(unsigned int controller, unsigned int button, InputType inputType, std::unique_ptr<Command> pCommand);

		/*
		 The order of buttons should be right, left, up, down
		 */
		void BindDigital2DAxisCommand(unsigned int controller, const std::vector<unsigned int>& buttons, std::unique_ptr<Command> pCommand);

		template <class T>
		void BindAnalogCommand(unsigned int controller, bool leftJoystick, bool x, GameObject* pGameObject);
		void BindAnalogCommand(unsigned int controller, bool leftJoystick, bool x, std::unique_ptr<Command> pCommand);

		void BindAnalog2DAxisCommand(unsigned int controller, bool left, std::unique_ptr<Command> pCommand);

		glm::vec2 GetTwoDirectionalAxis(Command* pCommand) const;
	private:
		struct InputDigital
		{
			unsigned int controllerIdx{};
			unsigned int button{};
			InputType inputType{};
		};
		struct InputAnalog
		{
			unsigned int controllerIdx{};
			bool left{};
			bool x{};
		};

		void AddControllersIfNeeded(unsigned int controller);

		glm::vec2 GetTwoDirectionalDigitalAxis(const std::vector<InputDigital>& inputVector) const;
		glm::vec2 GetTwoDirectionalAnalogAxis(const std::vector<InputAnalog>& inputVector) const;

		std::vector<std::pair<std::unique_ptr<Command>, std::vector<InputAnalog>>> m_pBindedAnalogCommands{};
		std::vector<std::pair<std::unique_ptr<Command>, std::vector<InputDigital>>> m_pBindedDigitalCommands{};
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
	};

	template<class T>
	inline void InputManager::BindDigitalCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		// Add controllers if the controllerIdx is higher then the amount of controllers available
		AddControllersIfNeeded(controller);
		
		// Try finding a command/input pair of type T
		const auto it{ std::find_if(begin(m_pBindedDigitalCommands), end(m_pBindedDigitalCommands), [](const auto& pBindedCommand)
			{
				return typeid(*pBindedCommand.first.get()) == typeid(T);
			})};


		// If a command/input pair has been found, bind the new input to this command
		if (it != m_pBindedDigitalCommands.end())
		{
			it->second.push_back(InputDigital{ controller, button, inputType });
			return;
		}

		// Create a new command
		m_pBindedDigitalCommands.push_back(std::make_pair(std::make_unique<T>(pGameObject), std::vector<InputDigital>{ InputDigital{ controller, button, inputType } }));
	}

	template<class T>
	inline void InputManager::BindAnalogCommand(unsigned int controller, bool leftJoystick, bool x, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		// Add controllers if the controllerIdx is higher then the amount of controllers available
		AddControllersIfNeeded(controller);

		// Try finding a command/input pair of type T
		const auto it{ std::find_if(begin(m_pBindedDigitalCommands), end(m_pBindedDigitalCommands), [](const auto& pBindedCommand)
			{
				if (std::is_same<T*, decltype(pBindedCommand.first.get())>()) return true;
			}) };

		// If a command/input pair has been found, bind the new input to this command
		if (it != m_pBindedDigitalCommands.end())
		{
			it->second.push_back(InputAnalog{ controller, leftJoystick, x });
			return;
		}

		// Create a new command
		m_pBindedAnalogCommands.push_back(std::make_pair(std::make_unique<T>(pGameObject), std::vector<InputAnalog>{ InputAnalog{ controller, leftJoystick, x } }));
	}
}
