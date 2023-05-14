#include "InstantAudioSource.h"

#include "ServiceLocator.h"
#include "AudioSystem.h"

void that::InstantAudioSource::SetData(const std::string& filePath, float volume)
{
	m_File = filePath;
	m_Volume = volume;
}

void that::InstantAudioSource::Init()
{
	AudioSystem& audio{ ServiceLocator::GetAudio() };

	audio.Load(m_File);
	audio.Play(m_File, m_Volume);
}
