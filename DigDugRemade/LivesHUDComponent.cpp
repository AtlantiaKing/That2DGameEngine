#include "LivesHUDComponent.h"

#include "EventQueue.h"
#include "GameObject.h"

#include "TextComponent.h"

#include <sstream>



void digdug::LivesHUDComponent::Notify(const HealthComponent& pHealth)
{
	std::stringstream hudText{};
	hudText << pHealth.GetHealth() << " lifes left";

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}

void digdug::LivesHUDComponent::OnDestroy()
{
	m_pHealth->OnHealthUpdate().RemoveListener(this);
}

void digdug::LivesHUDComponent::Display(that::GameObject* pPlayer)
{
	m_pHealth = pPlayer->GetComponent<HealthComponent>();
	m_pHealth->OnHealthUpdate().AddListener(this);

}
