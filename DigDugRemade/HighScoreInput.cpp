#include "HighScoreInput.h"

#include "HighScoreLetter.h"
#include "HighScoreButton.h"

#include "LambdaCommand.h"

#include "GameData.h"
#include "SceneManager.h"
#include "InputManager.h"

void digdug::HighScoreInput::Init()
{
	that::InputManager::GetInstance().BindDigitalCommand('d', that::InputManager::InputType::ONBUTTONDOWN, 
		std::make_unique<that::LambdaCommand>([this]() 
			{
				const int nrLetters{ static_cast<int>(m_pLetters.size()) };

				if (m_CurLetter == nrLetters)
					m_pCompleteButton->SetEnabled(false);
				else
					m_pLetters[m_CurLetter]->SetEnabled(false);

				++m_CurLetter;

				if (m_CurLetter == nrLetters)
				{
					m_pCompleteButton->SetEnabled(true);
				}
				else
				{
					if (m_CurLetter > nrLetters) m_CurLetter = 0;
					m_pLetters[m_CurLetter]->SetEnabled(true);
				}
			}));

	that::InputManager::GetInstance().BindDigitalCommand('a', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]() 
			{
				const int nrLetters{ static_cast<int>(m_pLetters.size()) };

				if (m_CurLetter == nrLetters)
					m_pCompleteButton->SetEnabled(false);
				else
					m_pLetters[m_CurLetter]->SetEnabled(false);

				--m_CurLetter;

				if (m_CurLetter < 0) m_CurLetter = nrLetters;

				if (m_CurLetter == nrLetters)
					m_pCompleteButton->SetEnabled(true);
				else
					m_pLetters[m_CurLetter]->SetEnabled(true);
			}));

	that::InputManager::GetInstance().BindDigitalCommand('w', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				const int nrLetters{ static_cast<int>(m_pLetters.size()) };

				if (m_CurLetter == nrLetters) return;

				m_pLetters[m_CurLetter]->MarkerUp();
			}));

	that::InputManager::GetInstance().BindDigitalCommand('s', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				const int nrLetters{ static_cast<int>(m_pLetters.size()) };

				if (m_CurLetter == nrLetters) return;

				m_pLetters[m_CurLetter]->MarkerDown();
			}));

	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				if(m_CurLetter == static_cast<int>(m_pLetters.size())) that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetMainMenuScene());
			}));
}

void digdug::HighScoreInput::OnDestroy()
{
	that::InputManager::GetInstance().Clear();
}

void digdug::HighScoreInput::InputUp()
{
	m_pLetters[m_CurLetter]->MarkerUp();
}

void digdug::HighScoreInput::InputDown()
{
	m_pLetters[m_CurLetter]->MarkerDown();
}

void digdug::HighScoreInput::SetLetters(const std::vector<HighScoreLetter*>& pLetters)
{
	m_pLetters = pLetters;
}

void digdug::HighScoreInput::SetDoneButton(HighScoreButton* pButton)
{
	m_pCompleteButton = pButton;
}
