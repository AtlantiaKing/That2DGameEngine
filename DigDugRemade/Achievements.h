#pragma once

#include "EventListener.h"
#include "Enemy.h"

#include "Singleton.h"

namespace digdug
{
	// Defining our achievements
	enum class Achievements
	{
		ACH_WIN_ONE_GAME = 0
	};

	class Player;

	class AchievementsUnlock final : public that::Singleton<AchievementsUnlock>, public that::EventListener<EnemyDeathEvent>
	{
	public:
		AchievementsUnlock();
		~AchievementsUnlock() = default;

		virtual void OnEvent(EnemyDeathEvent* e) override;

		void AddPlayer(Player* pPlayer);
	private:
		std::vector<Player*> m_pPlayers{};
	};
}