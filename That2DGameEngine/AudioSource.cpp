#include "AudioSource.h"

#include "GameObject.h"

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
	m_IsPlaying = true;

	AudioSystem& audio{ ServiceLocator::GetAudio() };

	if (!CheckSoundExist()) return;

	audio.SetLooping(m_Id, m_ShouldLoop);
	if (m_Enabled && GetOwner()->IsActive())
	{
		audio.Play(m_Id, m_Volume);
	}
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

	return m_Id != UINT_MAX;
}

void that::AudioSource::Stop()
{
	if (m_Id == UINT_MAX) return;

	ServiceLocator::GetAudio().Stop(m_Id);
	m_IsPlaying = false;
}

void that::AudioSource::ChangePlayState(bool paused)
{
	if (m_Id == UINT_MAX)
	{
		if (paused) return;

		if (!CheckSoundExist()) return;
	}

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

void that::AudioSource::OnEnable()
{
	if (m_IsPlaying) ChangePlayState(true);
}

void that::AudioSource::Update()
{
	if (m_IsPlaying && m_Id == UINT_MAX)
	{
		AudioSystem& audio{ ServiceLocator::GetAudio() };

		if (!CheckSoundExist()) return;

		audio.SetLooping(m_Id, m_ShouldLoop);
		if (m_Enabled && GetOwner()->IsActive())
		{
			audio.Play(m_Id, m_Volume);
		}
	}
}

void that::AudioSource::OnDisable()
{
	if (m_IsPlaying) ChangePlayState(false);
}

void that::AudioSource::OnDestroy()
{
	Stop();
}
