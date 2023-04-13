#pragma once

#include "Component.h"

namespace digdug
{
	class Enemy final : public that::Component
	{
	public:
		Enemy() = default;
		virtual ~Enemy() = default;

		Enemy(const Enemy& other) = delete;
		Enemy(Enemy&& other) = delete;
		Enemy& operator=(const Enemy& other) = delete;
		Enemy& operator=(Enemy&& other) = delete;

		void Kill();
	};
}