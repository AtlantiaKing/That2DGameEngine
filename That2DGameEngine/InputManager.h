#pragma once
#include "Singleton.h"

#include <memory>
#include <vector>

#include "Command.h"
#include "Controller.h"

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

		struct InputKey
		{
			unsigned int controllerIdx{};
			unsigned int button{};
			InputType inputType{};
		};
		struct InputAxis
		{
			unsigned int controllerIdx{};
			bool left{};
			bool x{};
		};

		bool ProcessInput();

		template <class T>
		void BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject);
		template <class T>
		void BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType);

		void BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType, std::unique_ptr<Command> pCommand);

		template <class T>
		void BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, GameObject* pGameObject);
		template <class T>
		void BindAxisCommand(unsigned int controller, bool leftJoystick, bool x);

		void BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, std::unique_ptr<Command> pCommand);

		float GetAxis(Command* pCommand);
	private:
		std::vector<std::pair<InputAxis, std::unique_ptr<Command>>> m_pBindedAxisCommands{};
		std::vector<std::pair<InputKey, std::unique_ptr<Command>>> m_pBindedButtonCommands{};
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
	};

	template<class T>
	inline void InputManager::BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		m_pBindedButtonCommands.push_back(std::make_pair(InputKey{ controller, button, inputType }, std::make_unique<T>(pGameObject)));
	}

	template<class T>
	inline void InputManager::BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		m_pBindedButtonCommands.push_back(std::make_pair(InputKey{ controller, button, inputType }, std::make_unique<T>()));
	}

	template<class T>
	inline void InputManager::BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		m_pBindedAxisCommands.push_back(std::make_pair(InputAxis{ controller, leftJoystick, x }, std::make_unique<T>(pGameObject)));
	}

	template<class T>
	inline void InputManager::BindAxisCommand(unsigned int controller, bool leftJoystick, bool x)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		m_pBindedAxisCommands.push_back(std::make_pair(InputAxis{ controller, leftJoystick, x }, std::make_unique<T>()));
	}
}
