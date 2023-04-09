#include "ScoreHUDComponent.h"

#include "Player.h"
#include <TextComponent.h>

#include "GameObject.h"

#include "EventQueue.h"

void digdug::ScoreHUDComponent::Init()
{
	that::EventQueue::GetInstance().AddListener<EnemyDeathEvent>(this);
}

void digdug::ScoreHUDComponent::OnEvent(EnemyDeathEvent* /*e*/)
{
	std::stringstream hudText{};
	hudText << "Score: " << m_pPlayer->GetScore();

	GetOwner()->GetComponent<that::TextComponent>()->SetText(hudText.str());
}
