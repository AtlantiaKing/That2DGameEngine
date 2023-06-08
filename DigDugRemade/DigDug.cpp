#include "DigDug.h"

#include "GameObject.h"

#include "BoxCollider.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "Transform.h"

#include "DigDugWalkingState.h"
#include "DigDugDeathState.h"
#include "DigDugRockDeathState.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "GameData.h"

#include "GridMoveCommand.h"
#include "ShootPumpCommand.h"
#include "PumpToEnemyCommand.h"

#include "ColliderLayers.h"

void digdug::DigDug::Init()
{
	m_SpawnPoint = GetTransform()->GetLocalPosition();

	ChangeState(std::make_unique<DigDugWalkingState>(GetOwner()));

	// Input
	that::InputManager::GetInstance().BindDigital2DAxisCommand({ 'd', 'a', 'w', 's' }, std::make_unique<GridMoveCommand>(this));
	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<ShootPumpCommand>(this));
	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTON, std::make_unique<PumpToEnemyCommand>(this));

	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().AddListener(this);
	GetOwner()->GetComponent<HealthComponent>()->OnDeath.AddListener(this);
}

void digdug::DigDug::OnDestroy()
{
	GetOwner()->GetComponent<that::BoxCollider>()->OnHitEvent().RemoveListener(this);
	GetOwner()->GetComponent<HealthComponent>()->OnDeath.RemoveListener(this);
	that::InputManager::GetInstance().Clear();
}

void digdug::DigDug::Update()
{
	ChangeState(m_pState->Update());
}

void digdug::DigDug::Notify(const that::CollisionData& collision)
{
	if (collision.pOther->GetLayer() == ENEMY_LAYER)
	{
		GetOwner()->GetComponent<HealthComponent>()->Hit();
		ChangeState(std::make_unique<DigDugDeathState>(GetOwner()));
	}
}

void digdug::DigDug::Notify(const that::GameObject&)
{
	GameData& gameData{ GameData::GetInstance() };

	if (gameData.TryNewHighScore(GetOwner()->GetComponent<ScoreComponent>()->GetScore()))
	{
		that::SceneManager::GetInstance().LoadScene(gameData.GetHighScoreScene());
	}
	else
	{
		that::SceneManager::GetInstance().LoadScene(gameData.GetMainMenuScene());
	}
}

void digdug::DigDug::Move(const glm::vec2& movementInput)
{
	ChangeState(m_pState->HandleInput(movementInput, false, false));
}

void digdug::DigDug::Pump(bool hold)
{
	ChangeState(m_pState->HandleInput(m_DefaultMovement, true, hold));
}

void digdug::DigDug::RockAttack()
{
	ChangeState(std::make_unique<DigDugRockDeathState>(GetOwner()));
}

void digdug::DigDug::ChangeState(std::unique_ptr<DigDugState> pState)
{
	if (!pState) return;

	if (m_pState) m_pState->StateEnd();

	m_pState = std::move(pState);
	m_pState->StateEnter();
}
