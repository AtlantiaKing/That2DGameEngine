#pragma once

#include "AudioSystem.h"
#include "CyclicBuffer.h"

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
		virtual void Play(AudioData* pAudioData) override;
		virtual void Pause(const unsigned int id) override;
		virtual void Unpause(const unsigned int id) override;
		virtual void Stop(const unsigned int id) override;
		virtual unsigned int Load(const std::string& path) override;
	private:
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
			SDLAudioEvent(AudioData* _pData, SDLAudioEventType _e)
				: pData{ _pData }
				, channel{ pData->id }
				, e{ _e }
			{}
			SDLAudioEvent(const unsigned int _channel, SDLAudioEventType _e)
				: channel{ _channel }
				, e{ _e }
			{}

			AudioData* pData{};
			unsigned int channel{};
			SDLAudioEventType e{};
		};

		CyclicBuffer<SDLAudioEvent, 10> m_EventBuffer{};
	};
}

