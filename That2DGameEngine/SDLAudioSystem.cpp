#include "SDLAudioSystem.h"

#include "ServiceLocator.h"

#include <SDL_mixer.h>
#include <SDL.h>

void that::SDLAudioSystem::Initialize()
{
	const int sdlInitResult{ SDL_Init(SDL_INIT_AUDIO) };
	if (sdlInitResult != 0)
	{
		std::stringstream errorMessage{};
		errorMessage << "SDL Audio Initialization failed" << SDL_GetError();

		Logger::LogError(errorMessage.str());
		return;
	}

	const int audioInitResult{ Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) };
	if (audioInitResult != 0)
	{
		std::stringstream errorMessage{};
		errorMessage << "SDL Mixer Initialization failed" << Mix_GetError();

		Logger::LogError(errorMessage.str());
		return;
	}

	m_AudioThread = std::jthread{ [this] { AudioThread(); } };
	m_AudioThread.detach();

	Mix_ChannelFinished(&OnSoundEndRoot);
}

void that::SDLAudioSystem::Play(const AudioData& pAudioData)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ pAudioData, SDLAudioEventType::PLAY });
	}

	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Pause(const unsigned int id)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ id, SDLAudioEventType::PAUSE });
	}

	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Unpause(const unsigned int id)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ id, SDLAudioEventType::UNPAUSE });
	}

	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Stop(const unsigned int id)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ id, SDLAudioEventType::STOP });
	}

	m_AudioCondition.notify_one();
}

unsigned int that::SDLAudioSystem::Load(const std::string& path)
{
	Mix_Chunk* pSound{ Mix_LoadWAV(path.c_str()) };

	if (!pSound) return UINT_MAX;

	SDLSound sdlSound{ pSound };

	std::lock_guard lock{ m_AudioMutex };
	m_pSounds.push_back(sdlSound);

	return static_cast<unsigned int>(m_pSounds.size()) - 1;
}

void that::SDLAudioSystem::AudioThread()
{
	while (true)
	{
		std::unique_lock lock(m_AudioMutex);
		m_AudioCondition.wait(lock, [&] { return m_EventBuffer.GetNrAssigned() > 0; });

		while (m_EventBuffer.GetNrAssigned() > 0)
		{
			SDLAudioEvent e{ m_EventBuffer.Pop() };
			switch (e.type)
			{
			case SDLAudioEventType::PLAY:
			{
				SDLSound& pSound{ m_pSounds[e.pData.id] };

				const int channel{ Mix_PlayChannel(-1, pSound.pData, 0) };
				if (channel == -1) break;

				Mix_Volume(channel, static_cast<int>(e.pData.volume * MIX_MAX_VOLUME));

				pSound.playingChannels.push_back(channel);
			}
				break;
			case SDLAudioEventType::PAUSE:
			{
				const SDLSound& sound{ m_pSounds[e.id] };

				for (int channel : sound.playingChannels)
				{
					Mix_Pause(channel);
				}
			}
				break;
			case SDLAudioEventType::UNPAUSE:
			{
				const SDLSound& sound{ m_pSounds[e.id] };

				for (int channel : sound.playingChannels)
				{
					Mix_Resume(channel);
				}
			}
				break;
			case SDLAudioEventType::STOP:
			{
				const SDLSound& sound{ m_pSounds[e.id] };

				for (int channel : sound.playingChannels)
				{
					Mix_HaltChannel(channel);
				}
			}
				break;
			}
		}
	}
}

void that::SDLAudioSystem::OnSoundEndRoot(int channel)
{
	ServiceLocator::GetAudio().OnSoundEnd(channel);
}

void that::SDLAudioSystem::OnSoundEnd(int channel)
{
	const auto soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds),
		[channel](const SDLSound& sound) { return std::find(begin(sound.playingChannels), end(sound.playingChannels), channel) != end(sound.playingChannels); }) };

	soundIt->playingChannels.erase(std::remove(begin(soundIt->playingChannels), end(soundIt->playingChannels), channel));
}
