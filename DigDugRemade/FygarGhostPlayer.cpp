#include "FygarGhostPlayer.h"

#include "GameObject.h"

#include "MoveCommand.h"

#include "InputManager.h"

digdug::FygarGhostPlayer::FygarGhostPlayer(that::GameObject* pFygar)
	: m_pFygarObj{ pFygar }
{
	m_pMoveCommand = that::InputManager::GetInstance().BindDigital2DAxisCommand({'d','a','w','s'}, std::make_unique<MoveCommand>(m_pFygarObj->GetTransform(), m_MoveSpeed));
}

digdug::FygarGhostPlayer::~FygarGhostPlayer()
{
	that::InputManager::GetInstance().Unbind(m_pMoveCommand);
}

std::unique_ptr<digdug::EnemyState> digdug::FygarGhostPlayer::Update()
{
	return nullptr;
}
