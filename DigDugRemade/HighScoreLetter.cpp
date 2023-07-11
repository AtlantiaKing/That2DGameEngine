#include "HighScoreLetter.h"

#include "GameObject.h"

#include "RenderComponent.h"
#include "TextComponent.h"

#include "Timer.h"
#include "GameData.h"

void digdug::HighScoreLetter::Init()
{
	m_pRenderer = GetOwner()->GetComponent<that::RenderComponent>();
}

void digdug::HighScoreLetter::Update()
{
	m_BlinkTime += that::Timer::GetInstance().GetElapsed();

	if (m_BlinkTime > m_TimeBetweenBlinks)
	{
		m_BlinkTime -= m_TimeBetweenBlinks;
		m_pRenderer->SetEnabled(!m_pRenderer->IsEnabled());
	}
}

void digdug::HighScoreLetter::OnEnable()
{
	m_BlinkTime = 0.0f;
}

void digdug::HighScoreLetter::OnDisable()
{
	if(m_pRenderer) m_pRenderer->SetEnabled(true);
}

void digdug::HighScoreLetter::SetIndex(int index)
{
	m_Index = index;
}

void digdug::HighScoreLetter::MarkerUp()
{
	++m_Letter;
	if (m_Letter > 'Z') m_Letter = 'A';
	GetOwner()->GetComponent<that::TextComponent>()->SetText({ m_Letter });
}

void digdug::HighScoreLetter::MarkerDown()
{
	--m_Letter;
	if (m_Letter < 'A') m_Letter = 'Z';
	GetOwner()->GetComponent<that::TextComponent>()->SetText({ m_Letter });
}

char digdug::HighScoreLetter::GetLetter() const
{
	return m_Letter;
}
