#include "LivesHUDComponent.h"

#include "GameObject.h"

#include "UIComponent.h"
#include "Transform.h"

#include "TextureManager.h"

#include <sstream>

void digdug::LivesHUDComponent::Update()
{
	if (!m_pParent) return;

	int curPlayerIdx{};
	for (that::GameObject* pChild : m_pParent->GetChildren())
	{
		if (pChild->GetTag() != "DigDug") continue;

		if (curPlayerIdx != m_PlayerIdx)
		{
			++curPlayerIdx;
			continue;
		}

		Display(pChild);
		m_pParent = nullptr;
		return;
	}
}

void digdug::LivesHUDComponent::Notify(const HealthComponent& health)
{
	const size_t nrTextures{ GetOwner()->GetChildCount() };
	const int healthLeft{ health.GetHealth() };

	for (int i{}; i < static_cast<int>(nrTextures); ++i)
	{
		if (i < healthLeft - 1) continue;

		GetOwner()->GetChild(i)->Destroy();
	}
}

void digdug::LivesHUDComponent::OnDestroy()
{
	if(m_pHealth) m_pHealth->OnHealthUpdate.RemoveListener(this);
}

void digdug::LivesHUDComponent::OnSubjectDestroy()
{
	m_pHealth = nullptr;
}

void digdug::LivesHUDComponent::SearchPlayer(that::GameObject* pPlayerParent, int playerIdx)
{
	m_pParent = pPlayerParent;
	m_PlayerIdx = playerIdx;
}

void digdug::LivesHUDComponent::Display(that::GameObject* pPlayer)
{
	m_pHealth = pPlayer->GetComponent<HealthComponent>();
	m_pHealth->OnHealthUpdate.AddListener(this);

	const auto& pTexture{ that::TextureManager::GetInstance().LoadTexture("HUD/Life.png") };
	const int textureWidth{ pTexture->GetSize().x };
	for (int i{}; i < m_pHealth->GetMaxHealth() - 1; ++i)
	{
		that::GameObject* pLife{ GetOwner()->CreateGameObject("Life") };
		that::UIComponent* pRenderer{ pLife->AddComponent<that::UIComponent>() };
		pRenderer->SetTexture(pTexture);
		pRenderer->SetPivot({ 0.0f, 0.5f });
		pLife->GetTransform()->SetLocalPosition(static_cast<float>(textureWidth * i), 0.0f);
	}
}
