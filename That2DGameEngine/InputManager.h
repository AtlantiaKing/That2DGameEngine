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
			ONBUTTON,
			AXIS
		};

		struct InputKey
		{
			unsigned int controllerIdx{};
			unsigned int button{};
			InputType inputType{};
		};

		bool ProcessInput();

		template <class T>
		void BindCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject);
		template <class T>
		void BindCommand(unsigned int controller, unsigned int button, InputType inputType);

		void BindCommand(unsigned int controller, unsigned int button, InputType inputType, std::unique_ptr<Command> pCommand);
	private:
		std::vector<std::pair<InputKey, std::unique_ptr<Command>>> m_pBindedCommands{};
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
	};

	template<class T>
	inline void InputManager::BindCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		m_pBindedCommands.push_back(std::make_pair(InputKey{ controller, button, inputType }, std::make_unique<T>(pGameObject)));
	}

	template<class T>
	inline void InputManager::BindCommand(unsigned int controller, unsigned int button, InputType inputType)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		m_pBindedCommands.push_back(std::make_pair(InputKey{ controller, button, inputType }, std::make_unique<T>()));
	}
}
