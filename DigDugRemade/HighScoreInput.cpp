#include "HighScoreInput.h"

#include "HighScoreLetter.h"
#include "HighScoreButton.h"

#include "LambdaCommand.h"

#include "GameData.h"
#include "SceneManager.h"
#include "InputManager.h"

void digdug::HighScoreInput::Init()
{
	// LEFT
	that::InputManager::GetInstance().BindDigitalCommand('d', that::InputManager::InputType::ONBUTTONDOWN, 
		std::make_unique<that::LambdaCommand>([this]() 
			{
				HandleInputHorizontal(true);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::DPAD_RIGHT, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputHorizontal(true);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(1, that::InputManager::GamepadButton::DPAD_RIGHT, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputHorizontal(true);
			}));

	// RIGHT
	that::InputManager::GetInstance().BindDigitalCommand('a', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputHorizontal(false);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::DPAD_LEFT, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputHorizontal(false);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(1, that::InputManager::GamepadButton::DPAD_LEFT, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputHorizontal(false);
			}));

	// UP
	that::InputManager::GetInstance().BindDigitalCommand('w', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputVertical(true);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::DPAD_UP, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputVertical(true);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(1, that::InputManager::GamepadButton::DPAD_UP, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputVertical(true);
			}));


	// DOWN
	that::InputManager::GetInstance().BindDigitalCommand('s', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputVertical(false);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::DPAD_DOWN, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputVertical(false);
			}));
	that::InputManager::GetInstance().BindDigitalCommand(1, that::InputManager::GamepadButton::DPAD_DOWN, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				HandleInputVertical(false);
			}));

	// ACCEPT
	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				if (m_CurLetter == static_cast<int>(m_pLetters.size())) that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetHighScoreTableScene());
			}));
	that::InputManager::GetInstance().BindDigitalCommand(0, that::InputManager::GamepadButton::A, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				if (m_CurLetter == static_cast<int>(m_pLetters.size())) that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetHighScoreTableScene());
			}));
	that::InputManager::GetInstance().BindDigitalCommand(1, that::InputManager::GamepadButton::A, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([this]()
			{
				if (m_CurLetter == static_cast<int>(m_pLetters.size())) that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetHighScoreTableScene());
			}));
}

void digdug::HighScoreInput::HandleInputHorizontal(bool left)
{
	const int nrLetters{ static_cast<int>(m_pLetters.size()) };

	if (left)
	{

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

	}
	else
	{

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

	}
}

void digdug::HighScoreInput::HandleInputVertical(bool up)
{
	const int nrLetters{ static_cast<int>(m_pLetters.size()) };

	if (m_CurLetter == nrLetters) return;

	if (up)
	{
		m_pLetters[m_CurLetter]->MarkerUp();
	}
	else
	{
		m_pLetters[m_CurLetter]->MarkerDown();
	}
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
