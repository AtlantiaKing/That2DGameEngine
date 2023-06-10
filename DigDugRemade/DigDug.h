#pragma once

#include "Component.h"
#include "Observer.h"

#include "PhysicsData.h"

#include "DigDugState.h"

#include "glm/vec2.hpp"

namespace digdug
{
	class HealthComponent;

	class DigDug final : public that::Component, public that::Observer<that::CollisionData>, public that::Observer<HealthComponent>
	{
	public:
		DigDug() = default;
		~DigDug() = default;

		DigDug(const DigDug& other) = delete;
		DigDug(DigDug&& other) = delete;
		DigDug& operator=(const DigDug& other) = delete;
		DigDug& operator=(DigDug&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;
		virtual void Update() override;

		virtual void Notify(const that::CollisionData& collision) override;
		virtual void Notify(const HealthComponent& healthComponent) override;

		void Move(const glm::vec2& movementInput);
		void Pump(bool hold);
		void RockAttack();
		void DisablePump();

		void SetPlayerIndex(int index);
		int GetPlayerIndex() const;

		const glm::vec2& GetSpawnPoint() { return m_SpawnPoint; }
	private:
		void ChangeState(std::unique_ptr<DigDugState> pState);

		const glm::vec2 m_DefaultMovement{};

		glm::vec2 m_SpawnPoint{};

		std::unique_ptr<DigDugState> m_pState{};

		int m_PlayerIndex{};
	};
}
