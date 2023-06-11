#include "DigDugWalkingState.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "GridTransform.h"	
#include "DigDug.h"

#include "InputManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "DigDugAudio.h"

#include "DigDugPumpState.h"

#include "GameData.h"
#include "GridMoveCommand.h"
#include "ShootPumpCommand.h"
#include "PumpToEnemyCommand.h"

#include <sstream>

digdug::DigDugWalkingState::DigDugWalkingState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
	, m_pTransform{ m_pPlayer->GetComponent<GridTransform>() }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugWalkingState::HandleInput(const glm::ivec2&, bool pumping, bool pumpHold)
{
	if (pumping && !pumpHold) return std::make_unique<DigDugPumpState>(m_pPlayer);

	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugWalkingState::Update()
{
	const auto& curPosition{ m_pPlayer->GetTransform()->GetLocalPosition() };

	if (abs(m_PrevPosition.x) < FLT_EPSILON && abs(m_PrevPosition.y) < FLT_EPSILON)
	{
		m_PrevPosition = curPosition;
		return nullptr;
	}

	bool isWalking{ abs(curPosition.x - m_PrevPosition.x) > FLT_EPSILON ||
					abs(curPosition.y - m_PrevPosition.y) > FLT_EPSILON };

	if (m_IsWalking != isWalking)
	{
		m_pPlayer->GetComponent<that::SpriteRenderer>()->SetTimePerTile(isWalking ? m_SpriteTime : FLT_MAX);

		DigDug* pDigDug{ m_pPlayer->GetComponent<DigDug>() };
		pDigDug->GetAudio()->SetWalking(pDigDug->GetPlayerIndex(), isWalking);
	}

	m_IsWalking = isWalking;
	m_PrevPosition = curPosition;

	return nullptr;
}

void digdug::DigDugWalkingState::StateEnter()
{
	DigDug* pDigDug{ m_pPlayer->GetComponent<DigDug>() };

	std::stringstream texturePath{};
	texturePath << "DigDug" << pDigDug->GetPlayerIndex() << "/Walking.png";

	const auto& pPlayerTexture{ that::TextureManager::GetInstance().LoadTexture(texturePath.str()) };
	m_pPlayer->GetComponent<that::SpriteRenderer>()->SetSprite(pPlayerTexture, 2, 1);

	if (pDigDug->GetPlayerIndex())
	{
		m_pMoveCommands.push_back(that::InputManager::GetInstance().BindAnalog2DAxisCommand(0, true, std::make_unique<GridMoveCommand>(m_pTransform)));
		m_pShootPumpCommands.push_back(that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::A, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pDigDug)));
	}
	else
	{
		m_pMoveCommands.push_back(that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'a', 'w', 's' }, std::make_unique<GridMoveCommand>(m_pTransform)));
		m_pShootPumpCommands.push_back(that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pDigDug)));

		const int isMultiplayer{ that::SceneManager::GetInstance().GetCurrentSceneIndex() != GameData::GetInstance().GetSinglePlayerScene() };

		m_pMoveCommands.push_back(that::InputManager::GetInstance().BindAnalog2DAxisCommand(isMultiplayer, true, std::make_unique<GridMoveCommand>(m_pTransform)));
		m_pShootPumpCommands.push_back(that::InputManager::GetInstance().BindDigitalCommand(isMultiplayer, that::InputManager::GamepadButton::A, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(pDigDug)));
	}
}

void digdug::DigDugWalkingState::StateEnd()
{
	for (auto pCommand : m_pMoveCommands)
	{
		that::InputManager::GetInstance().Unbind(pCommand);
	}
	for (auto pCommand : m_pShootPumpCommands)
	{
		that::InputManager::GetInstance().Unbind(pCommand);
	}

	DigDug* pDigDug{ m_pPlayer->GetComponent<DigDug>() };
	pDigDug->GetAudio()->SetWalking(pDigDug->GetPlayerIndex(), false);
}
