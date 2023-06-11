#include "FygarRoamingPlayer.h"

#include "GameObject.h"

#include "InputManager.h"

#include "LambdaCommand.h"
#include "GridMoveCommand.h"

#include "FygarAttackState.h"
#include "FygarGhostState.h"

#include "GridTransform.h"

digdug::FygarRoamingPlayer::FygarRoamingPlayer(that::GameObject* pFygar)
    : m_pFygarObj{ pFygar }
{
    m_pMoveCommand = that::InputManager::GetInstance().BindAnalog2DAxisCommand(0, true, std::make_unique<GridMoveCommand>(m_pFygarObj->GetComponent<GridTransform>(), true));
    m_pFireCommand = that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::A, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<that::LambdaCommand>([this]() { m_ActivateFire = true; }));
    m_pGhostCommand = that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::LEFT_SHOULDER, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<that::LambdaCommand>([this]() { m_Ghost = true; }));
}

digdug::FygarRoamingPlayer::~FygarRoamingPlayer()
{
    that::InputManager::GetInstance().Unbind(m_pMoveCommand);
}

std::unique_ptr<digdug::State> digdug::FygarRoamingPlayer::Update()
{
    if (m_ActivateFire) return std::make_unique<FygarAttackState>(m_pFygarObj);

    if (m_Ghost) return std::make_unique<FygarGhostState>(m_pFygarObj);

    return nullptr;
}
