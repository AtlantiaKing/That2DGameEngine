#include "FloatingScore.h"

#include "GameObject.h"

#include "Timer.h"

void digdug::FloatingScore::Update()
{
	m_CurTime += that::Timer::GetInstance().GetElapsed();
	if (m_CurTime > m_AliveTime) GetOwner()->Destroy();
}
