#pragma once

#include <string>

namespace that
{
	class AudioSystem
	{
	public:
		virtual ~AudioSystem() = default;
		virtual void Initialize() = 0;
		virtual void Play(unsigned int id, float volume) = 0;
		virtual void Play(const std::string& path, float volume) = 0;
		virtual void Pause(unsigned int id) = 0;
		virtual void Unpause(unsigned int id) = 0;
		virtual void Stop(unsigned int id) = 0;
		virtual void OnSoundEnd(int channel) = 0;
		virtual unsigned int GetIdFromName(const std::string& path) = 0;
		virtual void Load(const std::string& path) = 0;
	};
}
