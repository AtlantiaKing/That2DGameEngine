#pragma once

#include "Singleton.h"
#include <chrono>

namespace that
{
	class Time final : public Singleton<Time>
	{
	public:
		void Init();
		void Update();

		float GetElapsed() { return m_DeltaTime; };
		float GetTotal() { return m_TotalTime; };
	private:
		std::chrono::steady_clock::time_point m_PrevTime{};
		float m_DeltaTime{};
		float m_TotalTime{};
	};
}
