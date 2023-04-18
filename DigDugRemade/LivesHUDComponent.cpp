#include "LivesHUDComponent.h"

#include "EventQueue.h"
#include "GameObject.h"

#include "TextComponent.h"

#include <iostream>
#include <sstream>

void digdug::LivesHUDComponent::Notify(const HealthComponent& pHealth)
{
	std::stringstream hudText{};
	hudText << pHealth.GetHealth() << " lifes left";

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}

void digdug::LivesHUDComponent::Display(that::GameObject* pPlayer)
{
	pPlayer->GetComponent<HealthComponent>()->OnHealthUpdate().AddListener(this);
}
