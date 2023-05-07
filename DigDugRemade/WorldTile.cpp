#include "WorldTile.h"

#include "GameObject.h"
#include "TextureRenderer.h"
#include "ResourceManager.h"
#include "TextureMask.h"
#include "Transform.h"
#include "GridTransform.h"
#include "GridComponent.h"
#include <iostream>

void digdug::WorldTile::Init()
{
	that::GameObject* pOwner{ GetOwner() };

	m_pGridTr = pOwner->GetComponent<GridTransform>();

	m_pGrid = pOwner->GetParent()->GetComponent<GridComponent>();

	that::GameObject* pLeftMask{ pOwner->CreateGameObject("LeftMask") };
	that::GameObject* pRightMask{ pOwner->CreateGameObject("RightMask") };
	that::GameObject* pBottomMask{ pOwner->CreateGameObject("BottomMask") };
	that::GameObject* pTopMask{ pOwner->CreateGameObject("BottomMask") };

	auto pLeftTexture{ pLeftMask->AddComponent<that::TextureRenderer>() };
	pLeftTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("DiggedAreaLeft.png"));
	pLeftTexture->SetScale(2.0f);
	m_pLeftMask = pLeftMask->AddComponent<that::TextureMask>();
	m_pLeftMask->SetPercentage(true, 0.0f);

	auto pRightTexture{ pRightMask->AddComponent<that::TextureRenderer>() };
	pRightTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("DiggedAreaRight.png"));
	pRightTexture->SetScale(2.0f);
	pRightMask->GetTransform()->Rotate(180.0f);
	m_pRightMask = pRightMask->AddComponent<that::TextureMask>();
	m_pRightMask->SetPercentage(true, 0.0f);

	auto pBottomTexture{ pBottomMask->AddComponent<that::TextureRenderer>() };
	pBottomTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("DiggedAreaLeft.png"));
	pBottomTexture->SetScale(2.0f);
	pBottomMask->GetTransform()->Rotate(-90.0f);
	m_pBottomMask = pBottomMask->AddComponent<that::TextureMask>();
	m_pBottomMask->SetPercentage(true, 0.0f);

	auto pTopTexture{ pTopMask->AddComponent<that::TextureRenderer>() };
	pTopTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("DiggedAreaRight.png"));
	pTopTexture->SetScale(2.0f);
	pTopMask->GetTransform()->Rotate(90.0f);
	m_pTopMask = pTopMask->AddComponent<that::TextureMask>();
	m_pTopMask->SetPercentage(true, 0.0f);
}

void digdug::WorldTile::UpdatePlayer(const glm::ivec2& /*playerCell*/, const glm::ivec2& playerPosition, const glm::ivec2& direction, float playerSize)
{
	const int cellSize{ static_cast<int>(m_pGrid->GetCellSize()) };

	const auto& tilePos{ GetTransform()->GetLocalPosition() };
	const auto& textureSize{ GetOwner()->GetComponent<that::TextureRenderer>()->GetScaledTextureSize() };

	constexpr float epsilon{ 0.01f };
	if (playerPosition.x + playerSize < tilePos.x + epsilon || playerPosition.x + epsilon > tilePos.x + textureSize.x) return;
	if (playerPosition.y + playerSize < tilePos.y + epsilon || playerPosition.y + epsilon > tilePos.y + textureSize.y) return;

	if (direction.x != 0 && playerPosition.y % cellSize == 0)
	{
		const float curLeftMask{ m_pLeftMask->GetMask().x };
		if (playerPosition.x + (2 * (curLeftMask < 0.1f)) <= tilePos.x)
		{
			const int playerRight{ static_cast<int>(playerPosition.x + playerSize - tilePos.x) };

			const float percentage = std::max(static_cast<float>(playerRight - 2), 0.0f) / textureSize.x;

			if (curLeftMask < percentage)
				m_pLeftMask->SetPercentage(true, percentage);
		}

		const float curRightMask{ m_pRightMask->GetMask().x };
		if (playerPosition.x + playerSize - (2 * (curRightMask < 0.1f)) >= tilePos.x + textureSize.x)
		{
			const int playerLeft{ static_cast<int>(tilePos.x + textureSize.x - playerPosition.x) };

			const float percentage = std::max(static_cast<float>(playerLeft - 2), 0.0f) / textureSize.x;

			if (curRightMask < percentage)
				m_pRightMask->SetPercentage(true, percentage);
		}

		if (m_pLeftMask->GetMask().x > 0.5f && m_pRightMask->GetMask().x > 0.5f)
		{
			m_pLeftMask->SetPercentage(true, 1.0f);
			m_pRightMask->SetPercentage(true, 1.0f);
		}
	}
	else if (direction.y != 0 && playerPosition.x % cellSize == 0)
	{
		const float curBottomMask{ m_pTopMask->GetMask().x };
		if (playerPosition.y + (2 * (curBottomMask < 0.1f)) <= tilePos.y)
		{
			const int playerTop{ static_cast<int>(playerPosition.y + playerSize - tilePos.y) };

			const float percentage = std::max(static_cast<float>(playerTop - 2), 0.0f) / textureSize.x;

			if (curBottomMask < percentage)
				m_pTopMask->SetPercentage(true, percentage);
		}

		const float curTopMask{ m_pBottomMask->GetMask().x };
		if (playerPosition.y + playerSize - (2 * (curTopMask < 0.1f)) >= tilePos.y + textureSize.y)
		{
			const int playerBottom{ static_cast<int>(tilePos.y + textureSize.y - playerPosition.y) };

			const float percentage = std::max(static_cast<float>(playerBottom - 2), 0.0f) / textureSize.x;

			if (curTopMask < percentage)
				m_pBottomMask->SetPercentage(true, percentage);
		}

		if (m_pBottomMask->GetMask().x > 0.5f && m_pTopMask->GetMask().x > 0.5f)
		{
			m_pBottomMask->SetPercentage(true, 1.0f);
			m_pTopMask->SetPercentage(true, 1.0f);
		}
	}
}

bool digdug::WorldTile::IsValidPosition(const glm::vec2& position, const glm::ivec2& direction, float size)
{
	const auto& tilePos{ GetTransform()->GetLocalPosition() };
	const auto& textureSize{ GetOwner()->GetComponent<that::TextureRenderer>()->GetScaledTextureSize() };

	constexpr float epsilon{ 0.01f };
	if (position.x + size < tilePos.x + epsilon || position.x + epsilon > tilePos.x + textureSize.x) 
		return true;
	if (position.y + size < tilePos.y + epsilon || position.y + epsilon > tilePos.y + textureSize.y) 
		return true;

	if (direction.x > 0)
		return position.x + size >= tilePos.x + textureSize.x || position.x + size <= tilePos.x + m_pLeftMask->GetMask().x * textureSize.x;
	else if (direction.x < 0)
		return position.x <= tilePos.x || position.x >= tilePos.x + textureSize.x - m_pRightMask->GetMask().x * textureSize.x;
	else if (direction.y > 0)
		return position.y + size >= tilePos.y + textureSize.y || position.y + size <= tilePos.y + m_pTopMask->GetMask().x * textureSize.y;
	else if (direction.y < 0)
		return position.y <= tilePos.y || position.y >= tilePos.y + textureSize.y - m_pBottomMask->GetMask().x * textureSize.y;

	return false;
}
