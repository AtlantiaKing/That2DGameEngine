#pragma once

#include "AudioSystem.h"
#include <memory>

namespace that
{
	class DefaultAudioSystem : public AudioSystem
	{
	public:
		virtual ~DefaultAudioSystem() = default;
		virtual void Initialize() {}
		virtual void Play(AudioData*) {}
		virtual void Pause(unsigned int) {}
		virtual void Unpause(unsigned int) {}
		virtual void Stop(unsigned int) {}
		virtual unsigned int Load(const std::string&) { return UINT_MAX; }
	};

	class ServiceLocator final
	{
	public:
		static AudioSystem& GetAudio();
		template <typename T>
		static void RegisterAudioSystem();
	private:
		static std::unique_ptr<AudioSystem> m_pAudioSystem;
		static std::unique_ptr<DefaultAudioSystem> m_pDefaultAudioSystem;
	};
	template<typename T>
	inline void ServiceLocator::RegisterAudioSystem()
	{
		static_assert(std::is_base_of<AudioSystem, T>::value);
		m_pAudioSystem = std::make_unique<T>();
	}
}

