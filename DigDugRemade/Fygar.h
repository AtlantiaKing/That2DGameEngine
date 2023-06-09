#pragma once

#include "Component.h"
#include "Observer.h"

#include "EnemyState.h"

#include <vector>
#include <memory>

namespace digdug
{
	class HealthComponent;

	class Fygar final : public that::Component, public that::Observer<HealthComponent>
	{
	public:
		Fygar() = default;
		virtual ~Fygar() = default;

		Fygar(const Fygar& other) = delete;
		Fygar(Fygar&& other) = delete;
		Fygar& operator=(const Fygar& other) = delete;
		Fygar& operator=(Fygar&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;
		virtual void Update() override;
		virtual void Notify(const HealthComponent& health) override;

		void Start(const std::vector<that::GameObject*>& pPlayers, bool isPlayerController);
		that::GameObject* GetPlayer();

		bool IsPlayerControlled() const { return m_IsPlayerControlled; }

		void RockAttack();
	private:
		void ChangeState(std::unique_ptr<digdug::EnemyState> pState);

		std::vector<that::GameObject*> m_pPlayers{};
		int m_FollowingPlayerIdx{};

		bool m_IsPlayerControlled{};

		std::unique_ptr<EnemyState> m_pState{};
	};
}
