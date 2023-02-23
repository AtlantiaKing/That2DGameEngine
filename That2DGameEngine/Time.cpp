#include "Time.h"

void that::Time::Init()
{
	// Save the start time
	m_PrevTime = std::chrono::high_resolution_clock::now();
}

void that::Time::Update()
{
	// Get the current time
	const auto curTime{ std::chrono::high_resolution_clock::now() };

	// Update DeltaTime and TotalTime
	m_DeltaTime = std::chrono::duration<float>(curTime - m_PrevTime).count();
	m_TotalTime += m_DeltaTime;

	// Store the current time for the next frame
	m_PrevTime = curTime;
}
