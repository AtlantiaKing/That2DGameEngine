#pragma once

#include <string>

namespace that
{
	struct AudioData
	{
		unsigned int id{};
		float volume{};
	};

	class AudioSystem
	{
	public:
		virtual ~AudioSystem() = default;
		virtual void Initialize() = 0;
		virtual void Play(AudioData* pAudioData) = 0;
		virtual void Pause(unsigned int id) = 0;
		virtual void Unpause(unsigned int id) = 0;
		virtual void Stop(unsigned int id) = 0;
		virtual unsigned int Load(const std::string& path) = 0;
	};
}
