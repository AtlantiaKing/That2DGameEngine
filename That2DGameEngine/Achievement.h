#pragma once

namespace that
{
	struct Achievement_t
	{
		Achievement_t(int id, const char* idName)
			: m_eAchievementID{ id }
			, m_pchAchievementID{ idName }
		{
		}

		int m_eAchievementID{};
		const char* m_pchAchievementID{};
		char m_rgchName[128]{};
		char m_rgchDescription[256]{};
		bool m_bAchieved{};
		int m_iIconImage{};
	};
}