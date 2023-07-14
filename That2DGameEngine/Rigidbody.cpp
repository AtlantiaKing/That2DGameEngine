#include "Rigidbody.h"

#include "Logger.h"
#include "Timer.h"

#include "GameObject.h"

#include "BoxCollider.h"
#include "Transform.h"

void that::Rigidbody::Init()
{
	// Retrieve the collider on this gameobject
	m_pCollider = GetOwner()->GetComponent<BoxCollider>();

	// Log an error if no collider is found
	if (m_pCollider == nullptr) Logger::LogError("GameObject has Rigidbody but no BoxCollider", GetOwner());

	// Listen to any collision events
	m_pCollider->OnCollision.AddListener(this);

	// Retrieve the start position
	m_Position = GetTransform()->GetWorldPosition();
}

void that::Rigidbody::OnFrameStart()
{
	// Reset the previous step
	m_PrevStep = {};
}

void that::Rigidbody::Update()
{
	// Get data
	float elapsedSec{ Timer::GetInstance().GetElapsed() };

	// Apply gravity
	if (m_HasGravity)
	{
		if (m_HasConstantGravity)
			m_Velocity.y = m_Gravity;
		else 
			m_Velocity.y += m_Gravity * elapsedSec;
	}

	// Apply the velocity
	Move(m_Velocity * elapsedSec);
}

void that::Rigidbody::Notify(const CollisionData& collision)
{
	if (!m_HasCollisionCorrection) return;

	// Reset the velocity
	m_Velocity.x = abs(collision.fixStep.x) > FLT_EPSILON ? 0.0f : m_Velocity.x;
	m_Velocity.y = abs(collision.fixStep.y) > FLT_EPSILON ? 0.0f : m_Velocity.y;

	// Revert the previous movement
	m_Position += collision.fixStep;

	// Apply the position
	GetTransform()->SetWorldPosition(m_Position);
}

void that::Rigidbody::SetPosition(const glm::vec2 position, bool world)
{
	// Overwrite the current position
	if (world)
		m_Position = position;
	else
	{
		GameObject* pParent{ GetOwner()->GetParent() };

		if (pParent) m_Position = position + pParent->GetTransform()->GetWorldPosition();
		else m_Position = position;
	}

	// Apply the position
	GetTransform()->SetWorldPosition(m_Position);

	// Cancel out any movement the rigidbody might have done previous update
	m_PrevStep = {};
	m_Velocity = {};
}

void that::Rigidbody::Move(const glm::vec2 step)
{
	// Keep track of the velocity on update
	// We might need it again to revert this on collision
	m_PrevStep += step;

	// Apply velocity
	m_Position += step;

	// Apply the position
	GetTransform()->SetWorldPosition(m_Position);
}

void that::Rigidbody::Reset()
{
	m_Velocity = {};
	m_PrevStep = {};
}

void that::Rigidbody::AddForce(const glm::vec2& force, ForceMode mode)
{
	switch (mode)
	{
	case ForceMode::Force:
	{
		// Calculate the velocity using F = m * a = m * v / t => v = (F * t) / m
		m_Velocity += force * that::Timer::GetInstance().GetElapsed() / m_Mass;
		break;
	}
	case ForceMode::Impulse:
	{
		// Calculate the velocity using F = m * v => v = F / m
		m_Velocity += force / m_Mass;
		break;
	}
	}
}
