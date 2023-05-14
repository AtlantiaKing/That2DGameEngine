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

void that::SDLAudioSystem::Play(unsigned int id, float volume)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ AudioData{id,volume}, SDLAudioEventType::PLAY});
	}

	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Play(const std::string& path, float volume)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ AudioData{UINT_MAX,volume,path}, SDLAudioEventType::PLAY_NAME });
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

void that::SDLAudioSystem::Load(const std::string& path)
{
	{
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ AudioData{UINT_MAX,0.0f,path}, SDLAudioEventType::LOAD });
	}

	m_AudioCondition.notify_one();
}

unsigned int that::SDLAudioSystem::GetIdFromName(const std::string& path)
{
	while (m_EventBuffer.GetNrAssigned() > 0) {}

	const std::lock_guard lock{ m_AudioMutex };
	const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return path == sound.name; }) };
	if (soundIt == end(m_pSounds)) return UINT_MAX;

	return soundIt->id;
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
			case SDLAudioEventType::LOAD:
			{
				const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return e.pData.filePath == sound.name; }) };
				if(soundIt != end(m_pSounds)) break;

				LoadSound(e.pData.filePath);

				break;
			}
			case SDLAudioEventType::PLAY:
			{
				SDLSound& pSound{ m_pSounds[e.pData.id] };

				PlaySound(pSound, e.pData.volume);
				break;
			}
			case SDLAudioEventType::PLAY_NAME:
			{
				const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return e.pData.filePath == sound.name; }) };
				if (soundIt == end(m_pSounds))
				{
					LoadSound(e.pData.filePath);
					m_EventBuffer.Insert(e);
					break;
				}

				PlaySound(*soundIt, e.pData.volume);
				break;
			}
			case SDLAudioEventType::PAUSE:
			{
				const SDLSound& sound{ m_pSounds[e.id] };

				for (int channel : sound.playingChannels)
				{
					Mix_Pause(channel);
				}

				break;
			}
			case SDLAudioEventType::UNPAUSE:
			{
				const SDLSound& sound{ m_pSounds[e.id] };

				for (int channel : sound.playingChannels)
				{
					Mix_Resume(channel);
				}

				break;
			}
			case SDLAudioEventType::STOP:
			{
				const SDLSound& sound{ m_pSounds[e.id] };

				for (int channel : sound.playingChannels)
				{
					Mix_HaltChannel(channel);
				}

				break;
			}
			}
		}
	}
}

void that::SDLAudioSystem::LoadSound(const std::string& filePath)
{
	std::stringstream fullPath{};
	fullPath << "../Data/" << filePath;

	Mix_Chunk* pSound{ Mix_LoadWAV(fullPath.str().c_str()) };
	if (!pSound) return;

	SDLSound sdlSound{ static_cast<int>(m_pSounds.size()), pSound, filePath };
	m_pSounds.push_back(sdlSound);
}

void that::SDLAudioSystem::PlaySound(SDLSound& pSound, float volume)
{
	const int channel{ Mix_PlayChannel(-1, pSound.pData, 0) };
	if (channel == -1) return;

	Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));

	pSound.playingChannels.push_back(channel);
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
