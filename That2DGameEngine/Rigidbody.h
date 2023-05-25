#pragma once

#include "Component.h"

#include "Observer.h"
#include "PhysicsData.h"

#include "glm/vec2.hpp"

namespace that
{
	class BoxCollider;

	class Rigidbody final : public Component, Observer<CollisionData>
	{
	public:
		Rigidbody() = default;
		virtual ~Rigidbody() = default;

		Rigidbody(const Rigidbody& other) = delete;
		Rigidbody(Rigidbody&& other) = delete;
		Rigidbody& operator=(const Rigidbody& other) = delete;
		Rigidbody& operator=(Rigidbody other) = delete;

		virtual void Init() override;
		virtual void OnFrameStart() override;
		virtual void Update() override;
		virtual void Notify(const CollisionData& collision);
		
		void SetPosition(const glm::vec2 position, bool world = true);
		void Move(const glm::vec2 step);
		void SetGravity(float gravity) { m_Gravity = gravity; }
		void SetGravityEnabled(bool activated) { m_HasGravity = activated; }
		void SetConstantGravity(bool hasConstantGravity) { m_HasConstantGravity = hasConstantGravity; }
		void SetCollisionCorrection(bool hasCollisionCorrection) { m_HasCollisionCorrection = hasCollisionCorrection; }
		void SetMass(float mass) { m_Mass = mass; }
		void Reset();
	private:
		bool m_HasGravity{ true };
		bool m_HasConstantGravity{ false };
		bool m_HasCollisionCorrection{ true };
		float m_Gravity{ 9.81f };
		
		float m_Mass{ 1.0f };

		glm::vec2 m_Position{};
		glm::vec2 m_Velocity{};
		glm::vec2 m_PrevStep{};

		BoxCollider* m_pCollider{};
	};
}