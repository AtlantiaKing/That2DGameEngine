#include "RotatorComponent.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Time.h"
#include "Transform.h"

void that::RotatorComponent::Update()
{
	const auto pTransform{ GetTransform() };

	if (!pTransform) return;

	m_CurAngle += m_RotateSpeed * Time::GetInstance().GetElapsed() * static_cast<float>(M_PI) / 180.0f;

	const float x{ cosf(m_CurAngle) * m_DistanceFromPivot };
	const float y{ sinf(m_CurAngle) * m_DistanceFromPivot };

	pTransform->SetLocalPosition(x, y);
}
