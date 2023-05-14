#pragma once

#include "Component.h"

#include <string>

namespace that
{
	class InstantAudioSource final : public Component
	{
	public:
		InstantAudioSource() = default;
		virtual ~InstantAudioSource() = default;

		InstantAudioSource(const InstantAudioSource& other) = delete;
		InstantAudioSource(InstantAudioSource&& other) = delete;
		InstantAudioSource& operator=(const InstantAudioSource& other) = delete;
		InstantAudioSource& operator=(InstantAudioSource&& other) = delete;

		void SetData(const std::string& filePath, float volume = 1.0f);

		virtual void Init() override;
	private:
		std::string m_File{};
		float m_Volume{ 1.0f };
	};
}

