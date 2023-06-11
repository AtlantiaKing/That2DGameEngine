#include "MuteCommand.h"

#include "ServiceLocator.h"
#include "AudioSystem.h"

void digdug::MuteCommand::Execute()
{
	m_IsMuted = !m_IsMuted;

	if (m_IsMuted)
		that::ServiceLocator::GetAudio().Mute();
	else
		that::ServiceLocator::GetAudio().Unmute();
}
