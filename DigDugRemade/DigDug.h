#pragma once

#include "Component.h"
#include "Observer.h"

#include "PhysicsData.h"

#include "DigDugState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class DigDug final : public that::Component, public that::Observer<that::CollisionData>, public that::Observer<that::GameObject>
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
		virtual void Notify(const that::GameObject&) override;

		void Move(const glm::vec2& movementInput);
		void Pump(bool hold);
		void RockAttack();
	private:
		void ChangeState(std::unique_ptr<DigDugState> pState);

		const glm::vec2 m_DefaultMovement{};

		std::unique_ptr<DigDugState> m_pState{};
	};
}
