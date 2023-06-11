#include "FygarDeathState.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "SpriteRenderer.h"
#include "BoxCollider.h"

#include "Timer.h"
#include "TextureManager.h"

digdug::FygarDeathState::FygarDeathState(that::GameObject* pFygar)
	: m_pFygarObj{ pFygar }
{
}

std::unique_ptr<digdug::State> digdug::FygarDeathState::Update()
{
	m_WaitTime += that::Timer::GetInstance().GetElapsed();

	if (m_WaitTime > m_TimeTillDespawn)
	{
		m_pFygarObj->Destroy();
	}

	return nullptr;
}

void digdug::FygarDeathState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/Pumped.png") };
	that::SpriteRenderer* pSprite{ m_pFygarObj->GetComponent<that::SpriteRenderer>() };
	pSprite->SetSprite(pTexture, 4, 1);
	pSprite->SetTile(3);

	m_pFygarObj->GetComponent<that::BoxCollider>()->SetEnabled(false);
}

void digdug::FygarDeathState::StateEnd()
{
}
