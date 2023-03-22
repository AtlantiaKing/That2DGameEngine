#include "Controller.h"
#include <cstdlib>
#include <algorithm>

void that::Controller::Update()
{
    m_PreviousState = m_CurrentState;
    XInputGetState(m_ControllerIdx, &m_CurrentState);

    auto buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons };
    m_ButtonsDown = buttonChanges & m_CurrentState.Gamepad.wButtons;
    m_ButtonsUp = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool that::Controller::OnButtonDown(unsigned int button) const
{    
    return m_ButtonsDown & button;
}

bool that::Controller::OnButtonUp(unsigned int button) const
{
    return m_ButtonsUp & button;
}

bool that::Controller::OnButton(unsigned int button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}

float that::Controller::GetAxis(bool left, bool x) const
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
