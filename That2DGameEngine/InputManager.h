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

		/*
		 The order of buttons should be right, left, up, down
		 */
		void BindButton2DAxisCommand(unsigned int controller, const std::vector<unsigned int>& buttons, std::unique_ptr<Command> pCommand);

		template <class T>
		void BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, GameObject* pGameObject);
		template <class T>
		void BindAxisCommand(unsigned int controller, bool leftJoystick, bool x);
		void BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, std::unique_ptr<Command> pCommand);

		glm::vec2 GetTwoDirectionalAxis(Command* pCommand);
	private:
		std::vector<std::pair<std::unique_ptr<Command>, std::vector<InputAxis>>> m_pBindedAxisCommands{};
		std::vector<std::pair<std::unique_ptr<Command>, std::vector<InputKey>>> m_pBindedButtonCommands{};
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
	};

	template<class T>
	inline void InputManager::BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		const auto it{ std::find_if(begin(m_pBindedButtonCommands), end(m_pBindedButtonCommands), [](const auto& pBindedCommand)
			{
				return typeid(*pBindedCommand.first.get()) == typeid(T);
			})};

		if (it != m_pBindedButtonCommands.end())
		{
			it->second.push_back(InputKey{ controller, button, inputType });
			return;
		}

		m_pBindedButtonCommands.push_back(std::make_pair(std::make_unique<T>(pGameObject), std::vector<InputKey>{ InputKey{ controller, button, inputType } }));
	}

	template<class T>
	inline void InputManager::BindButtonCommand(unsigned int controller, unsigned int button, InputType inputType)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		const auto it{ std::find_if(begin(m_pBindedButtonCommands), end(m_pBindedButtonCommands), [](const auto& pBindedCommand)
			{
				if (std::is_same<T*, decltype(pBindedCommand.first.get())>()) return true;
			}) };

		if (it != m_pBindedButtonCommands.end())
		{
			it->second.push_back(InputKey{ controller, button, inputType });
			return;
		}

		m_pBindedButtonCommands.push_back(std::make_pair(std::make_unique<T>(), std::vector<InputKey>{ InputKey{ controller, button, inputType } }));
	}

	template<class T>
	inline void InputManager::BindAxisCommand(unsigned int controller, bool leftJoystick, bool x, GameObject* pGameObject)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		const auto it{ std::find_if(begin(m_pBindedButtonCommands), end(m_pBindedButtonCommands), [](const auto& pBindedCommand)
			{
				if (std::is_same<T*, decltype(pBindedCommand.first.get())>()) return true;
			}) };

		if (it != m_pBindedButtonCommands.end())
		{
			it->second.push_back(InputAxis{ controller, leftJoystick, x });
			return;
		}

		m_pBindedAxisCommands.push_back(std::make_pair(std::make_unique<T>(pGameObject), std::vector<InputAxis>{ InputAxis{ controller, leftJoystick, x } }));
	}

	template<class T>
	inline void InputManager::BindAxisCommand(unsigned int controller, bool leftJoystick, bool x)
	{
		static_assert(std::is_base_of<Command, T>(), "T needs to be derived from Command");

		if (controller >= m_pControllers.size()) m_pControllers.push_back(std::make_unique<Controller>(controller));

		const auto it{ std::find_if(begin(m_pBindedButtonCommands), end(m_pBindedButtonCommands), [](const auto& pBindedCommand)
			{
				if (std::is_same<T*, decltype(pBindedCommand.first.get())>()) return true;
			}) };

		if (it != m_pBindedButtonCommands.end())
		{
			it->second.push_back(InputAxis{ controller, leftJoystick, x });
			return;
		}

		m_pBindedAxisCommands.push_back(std::make_pair(std::make_unique<T>(), std::vector<InputAxis>{ InputAxis{ controller, leftJoystick, x } }));
	}
}
