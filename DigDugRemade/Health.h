#pragma once

#include "Component.h"

namespace digdug
{
	class Health final
	{
	public:
		Health(int health = 10) : m_Health{ health } {}
		virtual ~Health() = default;

		void Hit();
	private:
		int m_Health{};
	};
}
