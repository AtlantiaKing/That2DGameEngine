#include "StartRoundState.h"

#include "GameObject.h"
#include "Scene.h"

#include "TextureRenderer.h"
#include "Transform.h"
#include "GameState.h"

#include "InGameState.h"

#include "Timer.h"
#include "TextureManager.h"
#include "Window.h"

digdug::StartRoundState::StartRoundState(GameState* pState)
	: m_pState{ pState }
{
}

std::unique_ptr<digdug::State> digdug::StartRoundState::Update()
{
	m_WaitTimer += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTimer > m_TimeTillStartRound)
	{
		return std::make_unique<InGameState>(m_pState);
	}

	return nullptr;
}

void digdug::StartRoundState::StateEnter()
{
	const auto& screenSize{ that::Window::GetInstance().GetSize() };
	m_pReadyObject = m_pState->GetOwner()->GetScene()->CreateGameObject("ReadyHUD");
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("HUD/StartGame.png") };
	m_pReadyObject->AddComponent<that::TextureRenderer>()->SetTexture(pTexture);
	m_pReadyObject->GetTransform()->SetWorldScale(static_cast<float>(screenSize.y) / pTexture->GetSize().x);
	m_pReadyObject->GetTransform()->SetLocalPosition(screenSize.x / 2.0f, screenSize.y / 2.0f);
}

void digdug::StartRoundState::StateEnd()
{
	m_pReadyObject->Destroy();
}
