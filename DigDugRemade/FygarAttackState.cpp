#include "FygarAttackState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"

#include "FygarRoamingState.h"
#include "TextureManager.h"

digdug::FygarAttackState::FygarAttackState(that::GameObject* pFygar)
	: m_pFygarObj{ pFygar }
{
}

std::unique_ptr<digdug::EnemyState> digdug::FygarAttackState::Update()
{
	if (m_pFire->IsActive()) return nullptr;

	return std::make_unique<FygarRoamingState>(m_pFygarObj);
}

void digdug::FygarAttackState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Fygar/Attack.png") };
	m_pFygarObj->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 2, 1, 0.1f);

	m_pFire = m_pFygarObj->GetChild(0);
	m_pFire->SetActive(true);
}

void digdug::FygarAttackState::StateEnd()
{
}
