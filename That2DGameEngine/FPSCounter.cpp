#include "FPSCounter.h"
#include "TextComponent.h"
#include "Time.h"

void that::FPSCounter::Update()
{
	if (m_pText.expired())
	{
		m_pText = GetComponent<TextComponent>();

		if (m_pText.expired()) return;
	};

	auto pText{ m_pText.lock() };

	const float elapsedTime{ Time::GetInstance().GetElapsed() };

	m_AccuSec += elapsedTime;
	if (m_AccuSec >= m_UpdateInverval)
	{
		m_AccuSec -= m_UpdateInverval;

		std::stringstream fpsText{};

		fpsText << static_cast<int>(1.0f / Time::GetInstance().GetElapsed());
		fpsText << " FPS";

		pText->SetText(fpsText.str());
	}
}
