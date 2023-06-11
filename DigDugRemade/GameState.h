#pragma once

#include "Component.h"
#include "Observer.h"

#include <vector>

namespace that
{
	class GameObject;
	class Command;
}

namespace digdug
{
	class HealthComponent;

	class GameState final : public that::Component, public that::Observer<that::GameObject>
	{
	public:
		GameState() = default;
		virtual ~GameState() = default;

		GameState(const GameState& other) = delete;
		GameState(GameState&& other) = delete;
		GameState& operator=(const GameState& other) = delete;
		GameState& operator=(GameState&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void OnDestroy() override;
		virtual void Notify(const that::GameObject& pEnemy);

		void RestartRound() const;

	private:
		void InitEnemies();
		void InitPlayers();
		void GoToNextRound() const;

		bool m_Init{};

		std::vector<HealthComponent*> m_pEnemies{};
		std::vector<HealthComponent*> m_pPlayers{};

		bool m_Victory{};
		bool m_GameOver{};
		float m_WaitTimer{};
		const float m_TimeTillNextLevel{ 2.5f };

		that::Command* m_pSkipLevelCommand{};

		const int m_NrRounds{ 4 };
	};
}

