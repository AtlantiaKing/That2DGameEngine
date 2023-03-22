#include "Controller.h"

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
