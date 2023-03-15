#include "FPSCounter.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Time.h"
#include "Logger.h"

void that::FPSCounter::Update()
{
	// If no text component is assigned, try getting a text component from the parent
	//	If no text component is found on the parent, do nothing
	if (!m_pText)
	{
		m_pText = GetOwner()->GetComponent<TextComponent>();

		// Log a warning if the FPSCounter owner does not have a TextComponent
		if (!m_pText)
		{
			Logger::LogWarning("There is an FPSCounter component that doesn't have a TextComponent!");
			return;
		}
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

		m_pText->SetText(fpsText.str());
	}
}
