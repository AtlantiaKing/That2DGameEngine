#pragma once

#include "AudioSystem.h"
#include <memory>

namespace that
{
	class DefaultAudioSystem : public AudioSystem
	{
	public:
		virtual ~DefaultAudioSystem() = default;
		virtual void Play(const unsigned int, float) override {};
	};

	class ServiceLocator final
	{
	public:
		static AudioSystem& GetAudio();
		static void RegisterAudioSystem(std::unique_ptr<AudioSystem> pAudio);
	private:
		static std::unique_ptr<AudioSystem> m_pAudioSystem;
		static std::unique_ptr<DefaultAudioSystem> m_pDefaultAudioSystem;
	};
}

