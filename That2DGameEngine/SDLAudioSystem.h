#pragma once

#include "AudioSystem.h"

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

		virtual void Play(const unsigned int id, float volume) override;
	private:
	};
}

