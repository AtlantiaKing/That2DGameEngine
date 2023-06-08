#include "WorldTile.h"

#include "GameObject.h"

#include "TextureRenderer.h"
#include "TextureMask.h"
#include "Transform.h"
#include "GridTransform.h"
#include "GridComponent.h"

#include "TextureManager.h"

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
	pLeftTexture->SetTexture(that::TextureManager::GetInstance().LoadTexture("DiggedAreaLeft.png"));
	m_pLeftMask = pLeftMask->AddComponent<that::TextureMask>();
	m_pLeftMask->SetPercentage(true, 0.0f);

	auto pRightTexture{ pRightMask->AddComponent<that::TextureRenderer>() };
	pRightTexture->SetTexture(that::TextureManager::GetInstance().LoadTexture("DiggedAreaRight.png"));
	pRightMask->GetTransform()->Rotate(180.0f);
	m_pRightMask = pRightMask->AddComponent<that::TextureMask>();
	m_pRightMask->SetPercentage(true, 0.0f);

	auto pBottomTexture{ pBottomMask->AddComponent<that::TextureRenderer>() };
	pBottomTexture->SetTexture(that::TextureManager::GetInstance().LoadTexture("DiggedAreaLeft.png"));
	pBottomMask->GetTransform()->Rotate(-90.0f);
	m_pBottomMask = pBottomMask->AddComponent<that::TextureMask>();
	m_pBottomMask->SetPercentage(true, 0.0f);

	auto pTopTexture{ pTopMask->AddComponent<that::TextureRenderer>() };
	pTopTexture->SetTexture(that::TextureManager::GetInstance().LoadTexture("DiggedAreaRight.png"));
	pTopMask->GetTransform()->Rotate(90.0f);
	m_pTopMask = pTopMask->AddComponent<that::TextureMask>();
	m_pTopMask->SetPercentage(true, 0.0f);
}

void digdug::WorldTile::Update()
{
	const float bottomMask{ m_pBottomMask->GetMask().x };
	const float topMask{ m_pTopMask->GetMask().x };
	if (bottomMask + topMask >= 1.0f || bottomMask >= 1.0f || topMask >= 1.0f)
	{
		m_pBottomMask->SetPercentage(true, 1.0f);
		m_pTopMask->SetPercentage(true, 1.0f);
	}
	const float leftMask{ m_pLeftMask->GetMask().x };
	const float rightMask{ m_pRightMask->GetMask().x };
	if (leftMask + rightMask >= 1.0f || leftMask >= 1.0f || rightMask >= 1.0f)
	{
		m_pLeftMask->SetPercentage(true, 1.0f);
		m_pRightMask->SetPercentage(true, 1.0f);
	}
}

void digdug::WorldTile::UpdatePlayer(const glm::ivec2& /*playerCell*/, const glm::ivec2& playerPosition, const glm::ivec2& direction, float playerSize)
{
	const int cellSize{ static_cast<int>(m_pGrid->GetCellSize()) };

	const auto& tilePos{ GetTransform()->GetLocalPosition() };
	const auto& textureSize{ GetOwner()->GetComponent<that::TextureRenderer>()->GetTextureSize() };

	constexpr float epsilon{ 0.01f };
	if (playerPosition.x + playerSize < tilePos.x + epsilon || playerPosition.x + epsilon > tilePos.x + textureSize.x) return;
	if (playerPosition.y + playerSize < tilePos.y + epsilon || playerPosition.y + epsilon > tilePos.y + textureSize.y) return;

	if (direction.x != 0 && playerPosition.y % cellSize == 0)
	{
		const float curLeftMask{ m_pLeftMask->GetMask().x };
		const bool leftMaskUnused{ curLeftMask < m_UnusedMaskEpsilon };
		if (playerPosition.x + (m_BorderSize * leftMaskUnused) <= tilePos.x)
		{
			const int playerRight{ static_cast<int>(playerPosition.x + playerSize - tilePos.x) };

			const float percentage = std::max(static_cast<float>(playerRight - m_BorderSize), 0.0f) / textureSize.x;

			if (curLeftMask < percentage)
				m_pLeftMask->SetPercentage(true, percentage);
		}

		const float curRightMask{ m_pRightMask->GetMask().x };
		const bool rightMaskUnused{ curRightMask < m_UnusedMaskEpsilon };
		if (playerPosition.x + playerSize - (m_BorderSize * rightMaskUnused) >= tilePos.x + textureSize.x)
		{
			const int playerLeft{ static_cast<int>(tilePos.x + textureSize.x - playerPosition.x) };

			const float percentage = std::max(static_cast<float>(playerLeft - m_BorderSize), 0.0f) / textureSize.x;

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
		const bool bottomMaskUnused{ curBottomMask < m_UnusedMaskEpsilon };
		if (playerPosition.y + (m_BorderSize * bottomMaskUnused) <= tilePos.y)
		{
			const int playerTop{ static_cast<int>(playerPosition.y + playerSize - tilePos.y) };

			const float percentage = std::max(static_cast<float>(playerTop - m_BorderSize), 0.0f) / textureSize.x;

			if (curBottomMask < percentage)
				m_pTopMask->SetPercentage(true, percentage);
		}

		const float curTopMask{ m_pBottomMask->GetMask().x };
		const bool topMaskUnused{ curTopMask < m_UnusedMaskEpsilon };
		if (playerPosition.y + playerSize - (m_BorderSize * topMaskUnused) >= tilePos.y + textureSize.y)
		{
			const int playerBottom{ static_cast<int>(tilePos.y + textureSize.y - playerPosition.y) };

			const float percentage = std::max(static_cast<float>(playerBottom - m_BorderSize), 0.0f) / textureSize.x;

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

void digdug::WorldTile::SetMasks(float left, float right, float bottom, float top)
{
	m_pLeftMask->SetPercentage(true, left);
	m_pRightMask->SetPercentage(true, right);
	m_pBottomMask->SetPercentage(true, bottom);
	m_pTopMask->SetPercentage(true, top);
}

bool digdug::WorldTile::IsValidPosition(const glm::vec2& position, const glm::ivec2& direction, float size) const
{
	const auto& tilePos{ GetTransform()->GetLocalPosition() };
	const auto& textureSize{ GetOwner()->GetComponent<that::TextureRenderer>()->GetTextureSize() };

	if (position.x + size / 2.0f <= tilePos.x - textureSize.x / 2.0f || position.x - size / 2.0f >= tilePos.x + textureSize.x / 2.0f) 
		return true;
	if (position.y + size / 2.0f <= tilePos.y - textureSize.y / 2.0f || position.y - size / 2.0f >= tilePos.y + textureSize.x / 2.0f)
		return true;

	if (direction.x > 0)
	{
		bool help{ position.x + size / 2.0f >= tilePos.x + textureSize.x / 2.0f || position.x + size / 2.0f <= tilePos.x - textureSize.x / 2.0f + m_pLeftMask->GetMask().x * textureSize.x };
		return help;
	}
	else if (direction.x < 0)
	{
		bool help{ position.x - size / 2.0f <= tilePos.x - textureSize.x / 2.0f || position.x - size / 2.0f >= tilePos.x + textureSize.x / 2.0f - m_pRightMask->GetMask().x * textureSize.x };
		return help;
	}
	else if (direction.y > 0)
	{
		bool help{ position.y + size / 2.0f >= tilePos.y + textureSize.y / 2.0f || position.y + size / 2.0f <= tilePos.y - textureSize.y / 2.0f + m_pTopMask->GetMask().x * textureSize.y };
		return help;
	}
	else if (direction.y < 0)
	{
		bool help{ position.y - size / 2.0f <= tilePos.y - textureSize.y / 2.0f || position.y - size / 2.0f >= tilePos.y + textureSize.y / 2.0f - m_pBottomMask->GetMask().x * textureSize.y };
		return help;
	}

	return false;
}

bool digdug::WorldTile::IsOpen() const
{
	if (m_pBottomMask->GetMask().x > 0.5f && m_pTopMask->GetMask().x > 0.5f) return true;
	if (m_pLeftMask->GetMask().x > 0.5f && m_pRightMask->GetMask().x > 0.5f) return true;
	if (m_pBottomMask->GetMask().x > 0.9f || m_pTopMask->GetMask().x > 0.9f) return true;
	if (m_pLeftMask->GetMask().x > 0.9f || m_pRightMask->GetMask().x > 0.9f) return true;

	return false;
}

glm::ivec2 digdug::WorldTile::GetGridPosition() const
{
	const glm::ivec2& gridPos{ GetTransform()->GetLocalPosition() / m_pGrid->GetCellSize() };
	return gridPos;
}
