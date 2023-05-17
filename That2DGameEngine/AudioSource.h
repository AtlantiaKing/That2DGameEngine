#pragma once

#include "Component.h"

#include <string>

namespace that
{
	class AudioSource final : public Component
	{
	public:
		AudioSource() = default;
		virtual ~AudioSource() = default;

		AudioSource(const AudioSource& other) = delete;
		AudioSource(AudioSource&& other) = delete;
		AudioSource& operator=(const AudioSource& other) = delete;
		AudioSource& operator=(AudioSource&& other) = delete;

		void SetSound(unsigned int id);
		void SetSound(const std::string& filePath);
		void Reset();
		void SetVolume(float volume);
		void Play();
		void Stop();
		void ChangePlayState(bool paused);
		void SetLooping(bool shouldLoop);

		virtual void OnDestroy() override;
	private:
		bool CheckSoundExist();

		unsigned int m_Id{ UINT_MAX };
		std::string m_TargetSound{};
		float m_Volume{ 1.0f };
		bool m_IsPlaying{};
		bool m_ShouldLoop{};
	};
}

