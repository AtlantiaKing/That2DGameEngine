#pragma once

#include "Component.h"

namespace that
{
	class TextComponent;

	class FPSCounter final : public Component
	{
	public:
		FPSCounter() = default;
		virtual ~FPSCounter() = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		virtual void Update() override;
	private:
		TextComponent* m_pText{};
		float m_AccuSec{};
		const float m_UpdateInverval{ 0.5f };
	};
}

