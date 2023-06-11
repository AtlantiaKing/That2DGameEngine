#include "RockDeathState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"

#include "TextureManager.h"

digdug::RockDeathState::RockDeathState(that::GameObject* pRock)
	: m_pRock{ pRock }
{
}

std::unique_ptr<digdug::State> digdug::RockDeathState::Update()
{
	if (m_pSprite->GetTile() == 2) m_pRock->Destroy();

	return nullptr;
}

void digdug::RockDeathState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Rock/Break.png") };
	m_pSprite = m_pRock->GetComponent<that::SpriteRenderer>();
	m_pSprite->SetSprite(pTexture, 3, 1, 0.5f);
}

void digdug::RockDeathState::StateEnd()
{
}
