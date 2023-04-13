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
		void Heal();

		void SetHealth(int health);
		int GetHealth() const { return m_Health; }
	private:
		int m_Health{};
	};
}