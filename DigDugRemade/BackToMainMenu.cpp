#include "BackToMainMenu.h"

#include "InputManager.h"
#include "SceneManager.h"
#include "GameData.h"

#include "LambdaCommand.h"

void digdug::BackToMainMenu::Init()
{
	that::InputManager::GetInstance().BindAnyDigitalKeyboardCommand(that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<that::LambdaCommand>([]()
		{
			that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetMainMenuScene());
		}));
	that::InputManager::GetInstance().BindAnyDigitalGamepadCommand(0, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<that::LambdaCommand>([]()
		{
			that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetMainMenuScene());
		}));
	that::InputManager::GetInstance().BindAnyDigitalGamepadCommand(1, that::InputManager::InputType::ONBUTTONDOWN, std::make_unique<that::LambdaCommand>([]()
		{
			that::SceneManager::GetInstance().LoadScene(GameData::GetInstance().GetMainMenuScene());
		}));
}

void digdug::BackToMainMenu::OnDestroy()
{
	that::InputManager::GetInstance().Clear();
}
