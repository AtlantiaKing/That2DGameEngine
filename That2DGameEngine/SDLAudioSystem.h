#pragma once

#include "AudioSystem.h"
#include "CyclicBuffer.h"

#include <thread>
#include <mutex>
#include <vector>

struct Mix_Chunk;

namespace that
{
	class SDLAudioSystem final : public AudioSystem
	{
	public:
		SDLAudioSystem() = default;
		virtual ~SDLAudioSystem();

		SDLAudioSystem(const SDLAudioSystem& other) = delete;
		SDLAudioSystem(SDLAudioSystem&& other) = delete;
		SDLAudioSystem& operator=(const SDLAudioSystem& other) = delete;
		SDLAudioSystem& operator=(SDLAudioSystem&& other) = delete;

		virtual void Initialize() override;
		virtual void Play(unsigned int id, float volume) override;
		virtual void Play(const std::string& path, float volume) override;
		virtual void Pause(unsigned int id) override;
		virtual void Unpause(unsigned int id) override;
		virtual void Stop(unsigned int id) override;
		virtual void SetLooping(unsigned int id, bool shoudLoop) override;
		virtual void Load(const std::string& path) override;
		// Get the ID from a loaded file
		// This may block the current thread, use with caution
		virtual unsigned int GetIdFromName(const std::string& path);
		virtual void OnSoundEnd(int channel) override;
	private:
		enum class SDLAudioEventType
		{
			LOAD,
			PLAY,
			PLAY_NAME,
			PAUSE,
			UNPAUSE,
			STOP
		};

		struct SDLAudioEvent
		{
			SDLAudioEventType type{};
			unsigned int id{};
			float volume{};
			std::string filePath{};
		};

		struct SDLSound
		{
			unsigned int id{};
			Mix_Chunk* pData{};
			std::string name{};
			std::vector<int> playingChannels{};
			bool shouldLoop{};
		};

		void AudioThread();
		void LoadSound(const std::string& filePath);
		void PlaySound(SDLSound& sound, float volume);
		static void OnSoundEndRoot(int channel);

		CyclicBuffer<SDLAudioEvent, 10> m_EventBuffer{};
		std::vector<SDLSound> m_pSounds{};

		std::jthread m_AudioThread{};
		std::mutex m_AudioMutex{};
		std::condition_variable m_AudioCondition{};
	};
}

