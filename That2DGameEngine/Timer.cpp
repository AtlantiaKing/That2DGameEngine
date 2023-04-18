#include "Timer.h"

void that::Timer::Init()
{
	// Save the start time
	m_PrevTime = std::chrono::high_resolution_clock::now();
}

void that::Timer::Update()
{
	// Get the current time
	const auto curTime{ std::chrono::high_resolution_clock::now() };

	// Update DeltaTime and TotalTime
	m_DeltaTime = std::chrono::duration<float>(curTime - m_PrevTime).count();
	m_TotalTime += m_DeltaTime;

	// Make sure delta time never goes above maxDeltaTime
	m_DeltaTime = std::min(m_DeltaTime, m_MaxDeltaTime);

	// Store the current time for the next frame
	m_PrevTime = curTime;
}
