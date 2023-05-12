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
		virtual ~SDLAudioSystem() = default;

		SDLAudioSystem(const SDLAudioSystem& other) = delete;
		SDLAudioSystem(SDLAudioSystem&& other) = delete;
		SDLAudioSystem& operator=(const SDLAudioSystem& other) = delete;
		SDLAudioSystem& operator=(SDLAudioSystem&& other) = delete;

		virtual void Initialize() override;
		virtual void Play(const AudioData& pAudioData) override;
		virtual void Pause(const unsigned int id) override;
		virtual void Unpause(const unsigned int id) override;
		virtual void Stop(const unsigned int id) override;
		virtual unsigned int Load(const std::string& path) override;
		virtual void OnSoundEnd(int channel) override;
	private:
		void AudioThread();
		static void OnSoundEndRoot(int channel);

		enum class SDLAudioEventType
		{
			PLAY,
			PAUSE,
			UNPAUSE,
			STOP
		};

		struct SDLAudioEvent
		{
			SDLAudioEvent() = default;
			SDLAudioEvent(const AudioData& _pData, SDLAudioEventType _e)
				: pData{ _pData }
				, id{ pData.id }
				, type{ _e }
			{}
			SDLAudioEvent(const unsigned int _id, SDLAudioEventType _e)
				: id{ _id }
				, type{ _e }
			{}

			AudioData pData{};
			unsigned int id{};
			SDLAudioEventType type{};
		};

		struct SDLSound
		{
			Mix_Chunk* pData{};
			std::vector<int> playingChannels{};
		};

		CyclicBuffer<SDLAudioEvent, 10> m_EventBuffer{};
		std::vector<SDLSound> m_pSounds{};

		std::jthread m_AudioThread{};
		std::mutex m_AudioMutex{};
		std::condition_variable m_AudioCondition{};
	};
}

