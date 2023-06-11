#pragma once

#include "Component.h"

namespace that
{
	class Command;
}

namespace digdug
{
	class AudioMuter final : public that::Component
	{
	public:
		AudioMuter() = default;
		virtual ~AudioMuter() = default;

		AudioMuter(const AudioMuter& other) = delete;
		AudioMuter(AudioMuter&& other) = delete;
		AudioMuter& operator=(const AudioMuter& other) = delete;
		AudioMuter& operator=(AudioMuter&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;

	private:
		that::Command* m_pCommand{};
	};
}

