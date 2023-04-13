#pragma once

#include "Component.h"

#include "EventListener.h"
#include "Events.h"

namespace digdug
{
	class Player;

	class ScoreHUDComponent final : public that::Component, public that::EventListener<that::EntityDeathEvent>
	{
	public:
		ScoreHUDComponent() = default;
		virtual ~ScoreHUDComponent() = default;

		ScoreHUDComponent(const ScoreHUDComponent& other) = delete;
		ScoreHUDComponent(ScoreHUDComponent&& other) = delete;
		ScoreHUDComponent& operator=(const ScoreHUDComponent& other) = delete;
		ScoreHUDComponent& operator=(ScoreHUDComponent&& other) = delete;

		void Display(Player* pPlayer) { m_pPlayer = pPlayer; }

		virtual void Init() override;
		virtual void OnEvent(that::EntityDeathEvent*) override;
	private:
		Player* m_pPlayer{};
	};
}
