#include "HighScoreButton.h"

#include "GameObject.h"

#include "UIComponent.h"

#include "Timer.h"

void digdug::HighScoreButton::Init()
{
	m_pRenderer = GetOwner()->GetComponent<that::UIComponent>();
}

void digdug::HighScoreButton::Update()
{
	m_BlinkTime += that::Timer::GetInstance().GetElapsed();

	if (m_BlinkTime > m_TimeBetweenBlinks)
	{
		m_BlinkTime -= m_TimeBetweenBlinks;
		m_pRenderer->SetEnabled(!m_pRenderer->IsEnabled());
	}
}

void digdug::HighScoreButton::OnEnable()
{
	m_BlinkTime = 0.0f;
}

void digdug::HighScoreButton::OnDisable()
{
	if (m_pRenderer) m_pRenderer->SetEnabled(true);
}
