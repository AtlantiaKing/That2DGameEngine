#pragma once

#include "Component.h"
#include "EventListener.h"

#include "Enemy.h"

namespace digdug
{
	class Player;

	class ScoreHUDComponent final : public that::Component, public that::EventListener<EnemyDeathEvent>
	{
	public:
		ScoreHUDComponent() = default;
		virtual ~ScoreHUDComponent() = default;

		ScoreHUDComponent(const ScoreHUDComponent& other) = delete;
		ScoreHUDComponent(ScoreHUDComponent&& other) = delete;
		ScoreHUDComponent& operator=(const ScoreHUDComponent& other) = delete;
		ScoreHUDComponent& operator=(ScoreHUDComponent&& other) = delete;

		void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }

		virtual void Init() override;
		virtual void OnEvent(EnemyDeathEvent*) override;
	private:
		Player* m_pPlayer{};
	};
}
