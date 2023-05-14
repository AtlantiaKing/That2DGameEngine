#include "AudioSource.h"

#include "ServiceLocator.h"
#include "AudioSystem.h"
#include "Logger.h"

void that::AudioSource::SetSound(unsigned int id)
{
	m_Id = id;
}

void that::AudioSource::SetSound(const std::string& filePath)
{
	m_Id = UINT_MAX;
	m_TargetSound = filePath;
	ServiceLocator::GetAudio().Load(filePath);
}

void that::AudioSource::Reset()
{
	Stop();

	m_Id = UINT_MAX;
	m_TargetSound.resize(0);
}

void that::AudioSource::SetVolume(float volume)
{
	m_Volume = volume;
}

void that::AudioSource::Play()
{
	AudioSystem& audio{ ServiceLocator::GetAudio() };

	if (m_Id == UINT_MAX)
	{
		if (m_TargetSound.empty())
		{
			Logger::LogWarning("AudioSource is trying to play but has no target file assigned", GetOwner());
			return;
		}

		m_Id = audio.GetIdFromName(m_TargetSound);
	}

	audio.Play(m_Id, m_Volume);
}

void that::AudioSource::Stop() const
{
	if (m_Id == UINT_MAX) return;

	ServiceLocator::GetAudio().Stop(m_Id);
}

void that::AudioSource::ChangePlayState(bool paused) const
{
	if (m_Id == UINT_MAX) return;

	if(paused)
		ServiceLocator::GetAudio().Pause(m_Id);
	else
		ServiceLocator::GetAudio().Unpause(m_Id);
}
