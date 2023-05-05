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
	pBottomTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("DiggedAreaRight.png"));
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

void digdug::WorldTile::BindPlayer(GridTransform* pPlayer)
{
	m_pPlayer = pPlayer;

	m_pPlayer->GetOwner()->GetComponent<GridTransform>()->OnMove().AddListener(this);
}

void digdug::WorldTile::Notify(const glm::ivec2& change)
{
	const auto& textureSize{ GetOwner()->GetComponent<that::TextureRenderer>()->GetTextureSize() };

	if (change.y == 0 && abs(change.x) > 0)
	{
		if (m_pPlayer->GetCellPosition().y != m_pGridTr->GetCellPosition().y) return;

		auto playerTextureSize{ m_pPlayer->GetOwner()->GetComponent<that::TextureRenderer>()->GetTextureSize()};

		if (change.x < 0)
		{
			if (m_pPlayer->GetPosition().x > m_pGridTr->GetPosition().x + playerTextureSize.x) return;
			if (m_pPlayer->GetPosition().x < m_pGridTr->GetPosition().x) return;

			const int thisLeft{ m_pGridTr->GetPosition().x + textureSize.x };
			const int playerLeft{ thisLeft - m_pPlayer->GetPosition().x };

			const int thisSize{ textureSize.x };

			const float percentage{ static_cast<float>(playerLeft) / thisSize };

			if (m_pRightMask->GetMask().x < percentage) m_pRightMask->SetPercentage(true, percentage);
		}
		else
		{
			if (m_pPlayer->GetPosition().x > m_pGridTr->GetPosition().x) return;
			if (m_pPlayer->GetPosition().x + playerTextureSize.x < m_pGridTr->GetPosition().x) return;

			const int thisLeft{ m_pGridTr->GetPosition().x };
			const int playerRight{ m_pPlayer->GetPosition().x + playerTextureSize.x - thisLeft };

			const int thisSize{ textureSize.x };

			const float percentage{ static_cast<float>(playerRight) / thisSize };

			if(m_pLeftMask->GetMask().x < percentage) m_pLeftMask->SetPercentage(true, percentage);
		}
	}
	else if(abs(change.y) > 0)
	{
		if (m_pPlayer->GetCellPosition().x != m_pGridTr->GetCellPosition().x) return;

		const auto& playerTextureSize{ m_pPlayer->GetOwner()->GetComponent<that::TextureRenderer>()->GetTextureSize() };

		if (change.y < 0)
		{
			if (m_pPlayer->GetPosition().y > m_pGridTr->GetPosition().y + playerTextureSize.y) return;
			if (m_pPlayer->GetPosition().y < m_pGridTr->GetPosition().y) return;

			const int thisBottom{ m_pGridTr->GetPosition().y + textureSize.y };
			const int playerTop{ thisBottom - m_pPlayer->GetPosition().y };

			const int thisSize{ textureSize.y };

			const float percentage{ static_cast<float>(playerTop) / thisSize };

			if (m_pBottomMask->GetMask().x < percentage) m_pBottomMask->SetPercentage(true, percentage);
		}
		else
		{
			if (m_pPlayer->GetPosition().y > m_pGridTr->GetPosition().y) return;
			if (m_pPlayer->GetPosition().y + playerTextureSize.y < m_pGridTr->GetPosition().y) return;

			const int thisTop{ m_pGridTr->GetPosition().y };
			const int playerBottom{ m_pPlayer->GetPosition().y + playerTextureSize.y - thisTop };

			const int thisSize{ textureSize.y };

			const float percentage{ static_cast<float>(playerBottom) / thisSize };

			if (m_pTopMask->GetMask().x < percentage) m_pTopMask->SetPercentage(true, percentage);
		}
	}
}

void digdug::WorldTile::OnDestroy()
{
	m_pPlayer->OnMove().RemoveListener(this);
}
