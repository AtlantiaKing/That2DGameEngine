#include "Controller.h"
#include <cstdlib>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")

namespace that
{
    class Controller::ControllerImplementation
    {
    public:
        ControllerImplementation(unsigned int controllerIdx) : m_ControllerIdx{ controllerIdx } {}

        void Update()
        {
            m_PreviousState = m_CurrentState;
            XInputGetState(m_ControllerIdx, &m_CurrentState);

            auto buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons };
            m_ButtonsDown = buttonChanges & m_CurrentState.Gamepad.wButtons;
            m_ButtonsUp = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
        }
        void SetDeadZone(float percentage) { m_DeadZone = percentage; }

        bool OnButtonDown(unsigned int button) const
        {
            return m_ButtonsDown & button;
        }
        bool OnButtonUp(unsigned int button) const
        {
            return m_ButtonsUp & button;
        }
        bool OnButton(unsigned int button) const
        {
            return m_CurrentState.Gamepad.wButtons & button;
        }
        float GetAxis(bool left, bool x) const
        {
            constexpr float ceilPercentage{ 0.9f };
            constexpr SHORT ceilValue{ static_cast<SHORT>(ceilPercentage * MAXSHORT) };

            const auto axisValue
            {
                x
                ? (left ? m_CurrentState.Gamepad.sThumbLX : m_CurrentState.Gamepad.sThumbRX)
                : (left ? m_CurrentState.Gamepad.sThumbLY : m_CurrentState.Gamepad.sThumbRY)
            };

            const SHORT sign{ axisValue == 0 ? 0 : static_cast<SHORT>(abs(axisValue) / axisValue) };
            const SHORT deadZoneValue{ static_cast<SHORT>(m_DeadZone * MAXSHORT) };

            const SHORT clamped{ std::clamp(static_cast<SHORT>(abs(axisValue - sign)), deadZoneValue, ceilValue) };
            return static_cast<float>(clamped - deadZoneValue) / (ceilValue - deadZoneValue) * sign;
        }
    private:
        unsigned int m_ControllerIdx{};

        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};

        unsigned int m_ButtonsDown{};
        unsigned int m_ButtonsUp{};

        float m_DeadZone{ 0.25f };
    };
}

that::Controller::Controller(unsigned int controllerIdx)
    : m_pImpl{ new ControllerImplementation{ controllerIdx }}
{
}

that::Controller::~Controller()
{
    delete m_pImpl;
}

void that::Controller::Update()
{
    m_pImpl->Update();
}

void that::Controller::SetDeadZone(float percentage)
{
    m_pImpl->SetDeadZone(percentage);
}

bool that::Controller::OnButtonDown(unsigned int button) const
{
    return m_pImpl->OnButtonDown(button);
}

bool that::Controller::OnButtonUp(unsigned int button) const
{
    return m_pImpl->OnButtonUp(button);
}

bool that::Controller::OnButton(unsigned int button) const
{
    return m_pImpl->OnButton(button);
}

float that::Controller::GetAxis(bool left, bool x) const
{
    return m_pImpl->GetAxis(left, x);
}
