#include "Time.h"

void that::Time::Init()
{
	m_PrevTime = std::chrono::high_resolution_clock::now();
}

void that::Time::Update()
{
	const auto curTime{ std::chrono::high_resolution_clock::now() };

	m_DeltaTime = std::chrono::duration<float>(curTime - m_PrevTime).count();

	m_TotalTime += m_DeltaTime;

	m_PrevTime = curTime;
}
