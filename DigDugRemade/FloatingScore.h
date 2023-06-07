#pragma once

#include "Component.h"

namespace digdug
{
	class FloatingScore final : public that::Component
	{
	public:
		FloatingScore() = default;
		virtual ~FloatingScore() = default;

		FloatingScore(const FloatingScore& other) = delete;
		FloatingScore(FloatingScore&& other) = delete;
		FloatingScore& operator=(const FloatingScore& other) = delete;
		FloatingScore& operator=(FloatingScore&& other) = delete;

		virtual void Update() override;

	private:
		float m_CurTime{};
		const float m_AliveTime{ 2.0f };
	};
}
