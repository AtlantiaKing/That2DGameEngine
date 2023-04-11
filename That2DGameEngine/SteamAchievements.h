#pragma once

#include "Singleton.h"

#include "Achievement.h"

#include <memory>
#include <vector>
#include <string>

namespace that
{
	class SteamAchievements final : public that::Singleton<SteamAchievements>
	{
	public:
		SteamAchievements() = default; 
		~SteamAchievements() = default;

		SteamAchievements(const SteamAchievements& other) = delete;
		SteamAchievements(SteamAchievements&& other) = delete;
		SteamAchievements& operator=(const SteamAchievements& other) = delete;
		SteamAchievements& operator=(SteamAchievements&& other) = delete;

		void Init(const std::vector<Achievement_t>& achievements, bool resetAchievements = false);

		void SetAchievement(unsigned int achievementIdx);
	private:
		std::vector<Achievement_t> m_Achievements{};
	};
}