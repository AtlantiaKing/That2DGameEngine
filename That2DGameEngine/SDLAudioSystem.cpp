#include "SDLAudioSystem.h"
#include "SDL_mixer.h"

void that::SDLAudioSystem::Initialize()
{

}

void that::SDLAudioSystem::Play(AudioData* pAudioData)
{
	m_EventBuffer.Insert(SDLAudioEvent{ pAudioData, SDLAudioEventType::PLAY });
}

void that::SDLAudioSystem::Pause(const unsigned int id)
{
	m_EventBuffer.Insert(SDLAudioEvent{ id, SDLAudioEventType::PAUSE });
}

void that::SDLAudioSystem::Unpause(const unsigned int id)
{
	m_EventBuffer.Insert(SDLAudioEvent{ id, SDLAudioEventType::UNPAUSE });
}

void that::SDLAudioSystem::Stop(const unsigned int id)
{
	m_EventBuffer.Insert(SDLAudioEvent{ id, SDLAudioEventType::STOP });
}

unsigned int that::SDLAudioSystem::Load(const std::string&)
{
	return 0;
}
