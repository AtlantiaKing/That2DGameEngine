#pragma once

#include "Component.h"

namespace digdug
{
	class Health final : public that::Component
	{
	public:
		Health() = default;
		virtual ~Health() = default;

		void Hit();
		void SetHealth(int health) { m_Health = health; }
		int GetHealth() const { return m_Health; }
	private:
		int m_Health{};
	};
}