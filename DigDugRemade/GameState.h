#pragma once

#include "Component.h"

#include "State.h"

#include <vector>
#include <memory>

namespace that
{
	class GameObject;
	class Command;
}

namespace digdug
{
	class HealthComponent;

	class GameState final : public that::Component
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

		void RestartRound();
		void GoToNextRound() const;

		std::vector<HealthComponent*>& GetEnemies();
		std::vector<HealthComponent*>& GetPlayers();

		void Clear();

		bool IsInGame() const;

	private:
		void ChangeState(std::unique_ptr<State> pState);
		void InitEnemies();
		void InitPlayers();

		std::unique_ptr<State> m_pState{};

		bool m_Init{};

		std::vector<HealthComponent*> m_pEnemies{};
		std::vector<HealthComponent*> m_pPlayers{};

		that::Command* m_pSkipLevelCommand{};

		const int m_NrRounds{ 4 };
	};
}

