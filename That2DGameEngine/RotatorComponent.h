#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace that
{
	class RotatorComponent : public Component
	{
	public:
		RotatorComponent() = default;
		virtual ~RotatorComponent() = default;

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void SetRotateSpeed(float speed) { m_RotateSpeed = speed; };
		void SetRotateSpeedDegrees(float speed) { m_RotateSpeed = glm::radians(speed); };

		virtual void Update() override;
	private:
		const float m_DistanceFromPivot{ 20.0f };
		float m_RotateSpeed{ glm::radians(180.0f) };
		float m_CurAngle{};
	};
}

