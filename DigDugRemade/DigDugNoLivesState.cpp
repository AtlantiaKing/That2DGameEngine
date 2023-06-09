#include "DigDugNoLivesState.h"

#include "GameObject.h"

#include "ScoreComponent.h"

digdug::DigDugNoLivesState::DigDugNoLivesState(that::GameObject* pPlayer)
	: m_pPlayer{ pPlayer }
{
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugNoLivesState::HandleInput(const glm::ivec2&, bool, bool)
{
	return nullptr;
}

std::unique_ptr<digdug::DigDugState> digdug::DigDugNoLivesState::Update()
{
	return nullptr;
}

void digdug::DigDugNoLivesState::StateEnter()
{
	m_pPlayer->SetActive(false);
}

void digdug::DigDugNoLivesState::StateEnd()
{
}
