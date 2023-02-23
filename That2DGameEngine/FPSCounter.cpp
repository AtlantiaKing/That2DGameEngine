#include "FPSCounter.h"
#include "TextComponent.h"
#include "Time.h"

void that::FPSCounter::Update()
{
	// If no text component is assigned, try getting a text component from the parent
	//	If no text component is found on the parent, do nothing
	// TODO: Log a warning that no TextComponent is assigned to this FPSCounter
	if (m_pText.expired())
	{
		m_pText = GetComponent<TextComponent>();

		if (m_pText.expired()) return;
	};

	const float elapsedTime{ Time::GetInstance().GetElapsed() };

	// Every updateInterval, update the text to the FPS on this frame
	m_AccuSec += elapsedTime;
	if (m_AccuSec >= m_UpdateInverval)
	{
		m_AccuSec -= m_UpdateInverval;

		std::stringstream fpsText{};

		fpsText << static_cast<int>(1.0f / Time::GetInstance().GetElapsed());
		fpsText << " FPS";

		m_pText.lock()->SetText(fpsText.str());
	}
}
