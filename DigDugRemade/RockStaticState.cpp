#include "RockStaticState.h"

#include "GameObject.h"

#include "Transform.h"
#include "SpriteRenderer.h"
#include "GridComponent.h"

#include "TextureManager.h"

#include "RockWaitForPlayerState.h"

digdug::RockStaticState::RockStaticState(that::GameObject* pRock)
	: m_pRock{ pRock }
{
}

std::unique_ptr<digdug::State> digdug::RockStaticState::Update()
{
	glm::vec2 underPos{ m_pRock->GetTransform()->GetLocalPosition() };
	glm::ivec2 gridPos{ m_pRock->GetTransform()->GetLocalPosition() };
	gridPos /= static_cast<int>(m_pGrid->GetCellSize());
	gridPos += 1;
	underPos.y = gridPos.y * m_pGrid->GetCellSize();

	if (!m_pGrid->IsOpenPosition(underPos)) return nullptr;

	return std::make_unique<RockWaitForPlayerState>(m_pRock);
}

void digdug::RockStaticState::StateEnter()
{
	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("Rock/Default.png") };
	m_pRock->GetComponent<that::SpriteRenderer>()->SetSprite(pTexture, 1, 1);

	m_pGrid = m_pRock->GetParent()->GetComponent<GridComponent>();
}

void digdug::RockStaticState::StateEnd()
{
}
