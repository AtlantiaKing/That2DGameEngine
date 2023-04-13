#include "LivesHUDComponent.h"

#include "EventQueue.h"
#include "GameObject.h"

#include "TextComponent.h"
#include "Health.h"

#include <iostream>
#include <sstream>

void digdug::LivesHUDComponent::Init()
{
	that::EventQueue::GetInstance().AddListener<PlayerHitEvent>(this);
}

void digdug::LivesHUDComponent::OnEvent(PlayerHitEvent* e)
{
	if (!m_pPlayer) return;
	if (e->pPlayer != m_pPlayer) return;

	std::stringstream hudText{};
	hudText << e->pPlayer->GetComponent<Health>()->GetHealth() << " lifes left";

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}
