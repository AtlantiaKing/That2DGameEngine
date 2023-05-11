#include "ServiceLocator.h"

std::unique_ptr<that::DefaultAudioSystem> that::ServiceLocator::m_pDefaultAudioSystem{ std::make_unique<DefaultAudioSystem>() };
std::unique_ptr<that::AudioSystem> that::ServiceLocator::m_pAudioSystem{};

that::AudioSystem& that::ServiceLocator::GetAudio()
{
	return m_pAudioSystem.get() == nullptr ? *m_pDefaultAudioSystem.get() : *m_pAudioSystem.get();
}
