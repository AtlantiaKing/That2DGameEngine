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

	if (!CheckSoundExist()) return;

	audio.SetLooping(m_Id, m_ShouldLoop);
	audio.Play(m_Id, m_Volume);
	m_IsPlaying = true;
}

bool that::AudioSource::CheckSoundExist()
{
	AudioSystem& audio{ ServiceLocator::GetAudio() };

	if (m_Id == UINT_MAX)
	{
		if (m_TargetSound.empty())
		{
			Logger::LogWarning("AudioSource is trying to play but has no target file assigned", GetOwner());
			return false;
		}

		m_Id = audio.GetIdFromName(m_TargetSound);
	}

	return true;
}

void that::AudioSource::Stop()
{
	if (m_Id == UINT_MAX) return;

	ServiceLocator::GetAudio().Stop(m_Id);
	m_IsPlaying = false;
}

void that::AudioSource::ChangePlayState(bool paused)
{
	if (!CheckSoundExist()) return;

	if (paused)
	{
		ServiceLocator::GetAudio().Pause(m_Id);
	}
	else
	{
		if (m_IsPlaying)
			ServiceLocator::GetAudio().Unpause(m_Id);
		else
			Play();
	}
}

void that::AudioSource::SetLooping(bool shouldLoop)
{
	m_ShouldLoop = shouldLoop;
}

void that::AudioSource::OnDestroy()
{
	Stop();
}