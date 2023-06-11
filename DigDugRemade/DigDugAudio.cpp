#include "DigDugAudio.h"

#include "GameObject.h"

#include "AudioSource.h"

void digdug::DigDugAudio::SetWalking(int playerIdx, bool isWalking)
{
	m_ArePlayersWalking[playerIdx] = isWalking;
}

void digdug::DigDugAudio::Update()
{
	bool shouldPlayAudio{ m_ArePlayersWalking[0] || m_ArePlayersWalking[1] };

	if (shouldPlayAudio == m_IsPlayingSound) return;

	GetOwner()->GetComponent<that::AudioSource>()->ChangePlayState(!shouldPlayAudio);

	m_IsPlayingSound = shouldPlayAudio;
}
