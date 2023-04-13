#pragma once

#include "EventListener.h"
#include "Events.h"

#include "Singleton.h"

namespace digdug
{
	// Defining our achievements
	enum class Achievements
	{
		ACH_WIN_ONE_GAME = 0
	};

	class Player;

	class AchievementsUnlock final : public that::Singleton<AchievementsUnlock>, public that::EventListener<that::EntityDeathEvent>
	{
	public:
		AchievementsUnlock();
		~AchievementsUnlock() = default;

		virtual void OnEvent(that::EntityDeathEvent* e) override;

		void AddPlayer(Player* pPlayer);
	private:
		std::vector<Player*> m_pPlayers{};
	};
}