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
	m_pCollider->OnHitEvent().AddListener(this);

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
		constexpr float gravity{ 9.81f };
		m_Velocity.y += gravity * m_Mass * elapsedSec;
	}

	// Apply the velocity
	Move(m_Velocity * elapsedSec);
}

void that::Rigidbody::Notify(const CollisionData&)
{
	// Revert the previous movement
	m_Position -= m_PrevStep;
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
