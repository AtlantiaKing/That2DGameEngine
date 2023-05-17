#include "SDLAudioSystem.h"

#include "ServiceLocator.h"

#include <SDL_mixer.h>
#include <SDL.h>
#include <iostream>

that::SDLAudioSystem::~SDLAudioSystem()
{
	// Stop the audio thread
	m_AudioThread.request_stop();
	// Unblock the audio thread so it can be terminated
	m_AudioCondition.notify_all();

	// Release all the sounds that have been loaded
	for (SDLSound& sound : m_pSounds)
	{
		Mix_FreeChunk(sound.pData);
	}
}

void that::SDLAudioSystem::Initialize()
{
	// Initialize the SDL audio subsystem
	const int sdlInitResult{ SDL_Init(SDL_INIT_AUDIO) };
	if (sdlInitResult != 0)
	{
		std::stringstream errorMessage{};
		errorMessage << "SDL Audio Initialization failed : " << SDL_GetError();

		Logger::LogError(errorMessage.str());
		return;
	}

	// Open the default audio device for playback
	const int audioInitResult{ Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) };
	if (audioInitResult != 0)
	{
		std::stringstream errorMessage{};
		errorMessage << "SDL Mixer Initialization failed : " << Mix_GetError();

		Logger::LogError(errorMessage.str());
		return;
	}

	// Start the audio thread
	m_AudioThread = std::jthread{ [this] { AudioThread(); } };

	// Set the callback for when a channel is finished playing
	Mix_ChannelFinished(&OnSoundEndRoot);
}

void that::SDLAudioSystem::Play(unsigned int id, float volume)
{
	{
		// Add a play event that will use the id to the queue
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ SDLAudioEventType::PLAY, id, volume });
	}

	// Unblock the audio thread
	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Play(const std::string& path, float volume)
{
	{
		// Add a play event that will use the filepath to the queue
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ SDLAudioEventType::PLAY_NAME, UINT_MAX, volume, path });
	}

	// Unblock the audio thread
	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Pause(unsigned int id)
{
	{
		// Add a pause event to the queue
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ SDLAudioEventType::PAUSE, id });
	}

	// Unblock the audio thread
	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Unpause(unsigned int id)
{
	{
		// Add an unpause event to the queue
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ SDLAudioEventType::UNPAUSE, id });
	}

	// Unblock the audio thread
	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::Stop(unsigned int id)
{
	{
		// Add a stop event to the queue
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ SDLAudioEventType::STOP, id });
	}

	// Unblock the audio thread
	m_AudioCondition.notify_one();
}

void that::SDLAudioSystem::SetLooping(unsigned int id, bool shoudLoop)
{
	// Lock the event queue so no new sounds can be added
	const std::lock_guard lock{ m_AudioMutex };

	// Retrieve the sound that has the same file path
	const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return sound.id == id; }) };

	soundIt->shouldLoop = shoudLoop;
}

void that::SDLAudioSystem::Load(const std::string& path)
{
	{
		// Add a load event to the queue
		const std::lock_guard lock{ m_AudioMutex };
		m_EventBuffer.Insert(SDLAudioEvent{ SDLAudioEventType::LOAD, UINT_MAX, 0.0f, path });
	}

	// Unblock the audio thread
	m_AudioCondition.notify_one();
}

unsigned int that::SDLAudioSystem::GetIdFromName(const std::string& path)
{
	// Block this thread until the event buffer is empty 
	//		This ensures that the sound has been added
	while (m_EventBuffer.GetNrAssigned() > 0) {}

	// Lock the event queue so no new sounds can be added
	const std::lock_guard lock{ m_AudioMutex };

	// Retrieve the sound that has the same file path
	const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return path == sound.name; }) };

	// Return UINT_MAX if no sound has been found, else return the found soundid
	return soundIt == end(m_pSounds) ? UINT_MAX : soundIt->id;
}

void that::SDLAudioSystem::AudioThread()
{
	// Retrieve the stop token of this thread
	const std::stop_token& stopToken{ m_AudioThread.get_stop_token() };

	// Keep this thread alive as long as the thread has not been requested to stop
	while (!stopToken.stop_requested())
	{
		{
			// Lock the event queue
			std::unique_lock lock{ m_AudioMutex };

			// Wait for a change in the event queue
			m_AudioCondition.wait(lock, [&] { return m_EventBuffer.GetNrAssigned() > 0 || stopToken.stop_requested(); });
		}

		SDLAudioEvent e{};
		while (m_EventBuffer.GetNrAssigned() > 0)
		{
			{
				// Lock the event queue
				std::lock_guard lock{ m_AudioMutex };

				// Retrieve an event from the queue
				e = m_EventBuffer.Pop();
			}

			switch (e.type)
			{
			case SDLAudioEventType::LOAD:
			{
				// Check if the requested sound hasn't been loaded yet, if so do nothing
				const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return e.filePath == sound.name; }) };
				if (soundIt != end(m_pSounds)) break;

				// Load the sound using the path stored in the event
				LoadSound(e.filePath);

				break;
			}
			case SDLAudioEventType::PLAY:
			{
				// Retrieve the sound using the id stored in the event
				SDLSound& pSound{ m_pSounds[e.id] };

				// Play the sound at the volume stored in the event
				PlaySound(pSound, e.volume);
				break;
			}
			case SDLAudioEventType::PLAY_NAME:
			{
				// Retrieve the sound using the path stored in the event
				const auto& soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds), [&](const SDLSound& sound) { return e.filePath == sound.name; }) };

				// If no sound has been found yet
				if (soundIt == end(m_pSounds))
				{
					// Load the sound using the file path
					LoadSound(e.filePath);
					// Re-insert the play event in the queue
					m_EventBuffer.Insert(e);
					// Continue to the next event in the queue
					break;
				}

				// Play the sound at the volume stored in the event
				PlaySound(*soundIt, e.volume);
				break;
			}
			case SDLAudioEventType::PAUSE:
			{
				// Retrieve the sound using the id stored in the event
				const SDLSound& sound{ m_pSounds[e.id] };

				// Pause all the channels where this sound is being played
				for (int channel : sound.playingChannels)
				{
					Mix_Pause(channel);
				}

				break;
			}
			case SDLAudioEventType::UNPAUSE:
			{
				// Retrieve the sound using the id stored in the event
				const SDLSound& sound{ m_pSounds[e.id] };

				// Unpause all the channels where this sound is being played
				for (int channel : sound.playingChannels)
				{
					Mix_Resume(channel);
				}

				break;
			}
			case SDLAudioEventType::STOP:
			{
				// Retrieve the sound using the id stored in the event
				const SDLSound& sound{ m_pSounds[e.id] };

				// Stop all the channels where this sound is being played
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
	// Create the full file path
	std::stringstream fullPath{};
	fullPath << "../Data/" << filePath;

	// Try loading the sdl sound
	Mix_Chunk* pSound{ Mix_LoadWAV(fullPath.str().c_str()) };

	// Stop if loading the sound failed
	if (!pSound) return;

	// Create a sound object from the loaded sdl sound
	const SDLSound sdlSound{ static_cast<unsigned int>(m_pSounds.size()), pSound, filePath };

	// Add the newly created sound to the container of sounds
	m_pSounds.push_back(sdlSound);
}

void that::SDLAudioSystem::PlaySound(SDLSound& sound, float volume)
{
	// Play the sound at the first available channel
	const int channel{ Mix_PlayChannel(-1, sound.pData, sound.shouldLoop ? -1 : 0) };

	// Stop if no channel was available
	if (channel == -1) return;

	// Apply the volume to the channel where this sound is being played
	Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));

	// Add the channel to the sound
	sound.playingChannels.push_back(channel);
}

void that::SDLAudioSystem::OnSoundEndRoot(int channel)
{
	// This method is static so it can be used as callback in SDL_Mixer
	// We now have to call the non-static OnSoundEnd method
	ServiceLocator::GetAudio().OnSoundEnd(channel);
}

void that::SDLAudioSystem::OnSoundEnd(int channel)
{
	// Find the sound that is using the channel that has stopped playing
	const auto soundIt{ std::find_if(begin(m_pSounds), end(m_pSounds),
		[channel](const SDLSound& sound) { return std::find(begin(sound.playingChannels), end(sound.playingChannels), channel) != end(sound.playingChannels); }) };

	// Remove this channel from the container
	soundIt->playingChannels.erase(std::remove(begin(soundIt->playingChannels), end(soundIt->playingChannels), channel));
}
