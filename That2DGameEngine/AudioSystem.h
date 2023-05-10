#pragma once

namespace that
{
	class AudioSystem
	{
	public:
		virtual ~AudioSystem() = default;
		virtual void Play(const unsigned int id, const float volume) = 0;
	};
}
