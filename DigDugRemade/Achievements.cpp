#include "Achievements.h"

#include "Player.h"
#include "SteamAchievements.h"

#include "EventQueue.h"

digdug::AchievementsUnlock::AchievementsUnlock()
{
	that::EventQueue::GetInstance().AddListener<EnemyDeathEvent>(this);
}

void digdug::AchievementsUnlock::OnEvent(EnemyDeathEvent* /*e*/)
{
	constexpr int scoreForAchievement{ 500 };
	for (Player* pPlayer : m_pPlayers)
	{
		if (pPlayer->GetScore() > scoreForAchievement)
		{
			that::SteamAchievements::GetInstance().SetAchievement(static_cast<unsigned int>(Achievements::ACH_WIN_ONE_GAME));
		}
	}
}

void digdug::AchievementsUnlock::AddPlayer(Player* pPlayer)
{
	m_pPlayers.push_back(pPlayer);
}
