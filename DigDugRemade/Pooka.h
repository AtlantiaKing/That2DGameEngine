#pragma once

#include "Component.h"
#include "Observer.h"

#include "EnemyState.h"

#include <vector>
#include <memory>

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class HealthComponent;

	class Pooka final : public that::Component, public that::Observer<HealthComponent>
	{
	public:
		Pooka() = default;
		virtual ~Pooka() = default;

		Pooka(const Pooka& other) = delete;
		Pooka(Pooka&& other) = delete;
		Pooka& operator=(const Pooka& other) = delete;
		Pooka& operator=(Pooka&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;
		virtual void Update() override;
		virtual void Notify(const HealthComponent& health) override;

		void Start(const std::vector<that::GameObject*>& pPlayers);
		that::GameObject* GetPlayer();

		void RockAttack();

		void Reset();
	private:
		void ChangeState(std::unique_ptr<digdug::EnemyState> pState);

		std::vector<that::GameObject*> m_pPlayers{};
		int m_FollowingPlayerIdx{};
			
		std::unique_ptr<EnemyState> m_pState{};

		glm::vec2 m_Spawnpoint{};
	};
}
