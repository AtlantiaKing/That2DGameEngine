#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

namespace that
{
	class Controller
	{
	public:
		Controller(unsigned int controllerIdx) : m_ControllerIdx{ controllerIdx } {};
		~Controller() = default;

		void Update();
		void SetDeadZone(float percentage) { m_DeadZone = percentage; }

		bool OnButtonDown(unsigned int button) const;
		bool OnButtonUp(unsigned int button) const;
		bool OnButton(unsigned int button) const;
		float GetAxis(bool left, bool x) const;
	private:
		unsigned int m_ControllerIdx{};

		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};

		unsigned int m_ButtonsDown{};
		unsigned int m_ButtonsUp{};

		float m_DeadZone{ 0.25f };
	};
}

