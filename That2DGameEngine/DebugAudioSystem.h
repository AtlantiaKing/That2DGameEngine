#pragma once

#include "AudioSystem.h"
#include "Logger.h"

#include <memory>

namespace that
{
	template <typename T>
	class DebugAudioSystem final : public AudioSystem
	{
		static_assert(std::is_base_of<AudioSystem, T>::value, "Template argument of DebugAudioSystem should be an AudioSystem");

	public:
		DebugAudioSystem() = default;
		virtual ~DebugAudioSystem() = default;

		DebugAudioSystem(const DebugAudioSystem& other) = delete;
		DebugAudioSystem(DebugAudioSystem&& other) = delete;
		DebugAudioSystem& operator=(const DebugAudioSystem& other) = delete;
		DebugAudioSystem& operator=(DebugAudioSystem&& other) = delete;

		virtual void Initialize();
		virtual void Play(unsigned int id, float volume);
		virtual void Play(const std::string& path, float volume);
		virtual void Pause(unsigned int id);
		virtual void Unpause(unsigned int id);
		virtual void Stop(unsigned int id);
		virtual void OnSoundEnd(int channel);
		virtual unsigned int GetIdFromName(const std::string& path);
		virtual void Load(const std::string& path);
	private:
		std::unique_ptr<T> m_pAudioSystem{};
	};

	template<typename T>
	inline void DebugAudioSystem<T>::Initialize()
	{
		m_pAudioSystem = std::make_unique<T>();
		m_pAudioSystem->Initialize();
		Logger::Log("AudioSystem has been initialized");
	}

	template<typename T>
	inline void DebugAudioSystem<T>::Play(unsigned int id, float volume)
	{
		m_pAudioSystem->Play(id, volume);

		std::stringstream log{};
		log << "AudioSystem is now playing the sound with id " << id;
		Logger::Log(log.str());
	}

	template<typename T>
	inline void DebugAudioSystem<T>::Play(const std::string& path, float volume)
	{
		m_pAudioSystem->Play(path, volume);

		std::stringstream log{};
		log << "AudioSystem is now playing the sound with filepath " << path;
		Logger::Log(log.str());
	}

	template<typename T>
	inline void DebugAudioSystem<T>::Pause(unsigned int id)
	{
		m_pAudioSystem->Pause(id);

		std::stringstream log{};
		log << "AudioSystem is now pausing the sound with id " << id;
		Logger::Log(log.str());
	}

	template<typename T>
	inline void DebugAudioSystem<T>::Unpause(unsigned int id)
	{
		m_pAudioSystem->Unpause(id);

		std::stringstream log{};
		log << "AudioSystem is now unpausing the sound with id " << id;
		Logger::Log(log.str());
	}

	template<typename T>
	inline void DebugAudioSystem<T>::Stop(unsigned int id)
	{
		m_pAudioSystem->Stop(id);

		std::stringstream log{};
		log << "AudioSystem is now stopping the sound with id " << id;
		Logger::Log(log.str());
	}

	template<typename T>
	inline void DebugAudioSystem<T>::OnSoundEnd(int channel)
	{
		m_pAudioSystem->OnSoundEnd(channel);

		std::stringstream log{};
		log << "AudioSystem has been notified of the end of a sound on channel " << channel;
		Logger::Log(log.str());
	}

	template<typename T>
	inline unsigned int DebugAudioSystem<T>::GetIdFromName(const std::string& path)
	{
		const unsigned int id{ m_pAudioSystem->GetIdFromName(path) };

		std::stringstream log{};
		log << "AudioSystem has found a sound with path " << path << ". It has id " << id;
		Logger::Log(log.str());

		return id;
	}

	template<typename T>
	inline void DebugAudioSystem<T>::Load(const std::string& path)
	{
		m_pAudioSystem->Load(path);

		std::stringstream log{};
		log << "AudioSystem is loading a sound with path " << path;
		Logger::Log(log.str());
	}
}

