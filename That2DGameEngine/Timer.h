#pragma once

#include "Singleton.h"
#include <chrono>

namespace that
{
	class Timer final : public Singleton<Timer>
	{
	public:
		void Init();
		void Update();

		float GetElapsed() const { return m_DeltaTime; };
		float GetTotal() const { return m_TotalTime; };

		void SetMaxDeltaTime(float maxDeltaTime) { m_MaxDeltaTime = maxDeltaTime; }
	private:
		std::chrono::steady_clock::time_point m_PrevTime{};
		float m_DeltaTime{};
		float m_TotalTime{};
		float m_MaxDeltaTime{ 0.1f };
	};
}
