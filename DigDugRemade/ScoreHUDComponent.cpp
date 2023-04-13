#include "ScoreHUDComponent.h"

#include "Player.h"
#include "Enemy.h"
#include "TextComponent.h"

#include "GameObject.h"

#include "EventQueue.h"

void digdug::ScoreHUDComponent::Init()
{
	that::EventQueue::GetInstance().AddListener(this);
}

void digdug::ScoreHUDComponent::OnEvent(that::EntityDeathEvent* e)
{
	that::GameObject* pEntity{ e->pEntity };

	if (!pEntity || !pEntity->GetComponent<Enemy>()) return;

	std::stringstream hudText{};
	hudText << "Score: " << m_pPlayer->GetScore();

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}
