#include "RotatorComponent.h"
#include "Time.h"
#include "Transform.h"

void that::RotatorComponent::Update()
{
	const auto pTransform{ GetTransform() };
	if (!pTransform) return;

	m_CurAngle += m_RotateSpeed * Time::GetInstance().GetElapsed();

	const float x{ cosf(m_CurAngle) * m_DistanceFromPivot };
	const float y{ sinf(m_CurAngle) * m_DistanceFromPivot };

	pTransform->SetLocalPosition(x, y);
}
