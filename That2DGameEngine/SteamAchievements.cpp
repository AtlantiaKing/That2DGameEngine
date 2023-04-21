#include "SteamAchievements.h"

//#include "isteamuserstats.h"

void that::SteamAchievements::Init(const std::vector<Achievement_t>& achievements, bool /*resetAchievements*/)
{
	m_Achievements = achievements;

	/*if (resetAchievements)
	{
		for (const Achievement_t& achievement : m_Achievements)
		{
			SteamUserStats()->ClearAchievement(achievement.m_pchAchievementID);
		}
	}*/
}

void that::SteamAchievements::SetAchievement(unsigned int /*achievementIdx*/)
{
	/*auto pUserStats{ SteamUserStats() };
	pUserStats->SetAchievement(m_Achievements[achievementIdx].m_pchAchievementID);
	pUserStats->StoreStats();*/
}
