#include "Achievements.h"

#include "SteamAchievements.h"
#include "Player.h"
#include "GameObject.h"
#include "ScoreComponent.h"

#include "EventQueue.h"

digdug::AchievementsUnlock::AchievementsUnlock()
{
	that::EventQueue::GetInstance().AddListener<that::EntityDeathEvent>(this);
}

void digdug::AchievementsUnlock::OnEvent(that::EntityDeathEvent* /*e*/)
{
	constexpr int scoreForAchievement{ 500 };
	for (Player* pPlayer : m_pPlayers)
	{
		if (pPlayer->GetOwner()->GetComponent<ScoreComponent>()->GetScore() > scoreForAchievement)
		{
			that::SteamAchievements::GetInstance().SetAchievement(static_cast<unsigned int>(Achievements::ACH_WIN_ONE_GAME));
		}
	}
}

void digdug::AchievementsUnlock::AddPlayer(Player* pPlayer)
{
	m_pPlayers.push_back(pPlayer);
}
