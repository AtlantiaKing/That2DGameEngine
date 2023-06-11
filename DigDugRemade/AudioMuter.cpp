#include "AudioMuter.h"

#include "InputManager.h"

#include "MuteCommand.h"

void digdug::AudioMuter::Init()
{
	m_pCommand = that::InputManager::GetInstance().BindDigitalCommand('m', that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<MuteCommand>());
}

void digdug::AudioMuter::OnDestroy()
{
	that::InputManager::GetInstance().Unbind(m_pCommand);
}
