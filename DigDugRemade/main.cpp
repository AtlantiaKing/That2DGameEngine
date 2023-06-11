#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "Window.h"

//#include "DemoScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "VersusGameScene.h"
#include "CoOpGameScene.h"
#include "HighScoreScene.h"
#include "HighScoreTableScene.h"
#include "Logger.h"

#include <vector>

void Setup()
{
	that::Window::GetInstance().SetTitle("Programming 4 assignment");
	that::Window::GetInstance().SetWindowSize(610, 520);

	that::SceneManager::GetInstance().AddScene(digdug::MainMenuScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::GameScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::VersusGameScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::CoOpGameScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::HighScoreScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::HighScoreTable::Load);



	// CONTROLS:
	that::Logger::Log("");
	that::Logger::Log("=== CONTROLS DIG DUG ===");
	that::Logger::Log("");
	that::Logger::Log("===       MENU       ===");
	that::Logger::Log("Selection up/down/right/left : WASD or D-Pad");
	that::Logger::Log("Accept selection : Space bar or south button (A on XBox)");
	that::Logger::Log("");
	that::Logger::Log("===      INGAME      ===");
	that::Logger::Log("Mute : M");
	that::Logger::Log("Skip level : Page up");
	that::Logger::Log("");
	that::Logger::Log("===   SINGLEPLAYER   ===");
	that::Logger::Log("Move : WASD or left joystick");
	that::Logger::Log("Shoot pump/pump to enemy : Space bar or south button (A on XBox)");
	that::Logger::Log("");
	that::Logger::Log("===   MULTIPLAYER   ===");
	that::Logger::Log("=       Player1       =");
	that::Logger::Log("Move : WASD or left joystick (if a second controller is connected)");
	that::Logger::Log("Shoot pump/pump to enemy : Space bar or south button (A on XBox) (if a second controller is connected)");
	that::Logger::Log("=       Player2       =");
	that::Logger::Log("Move : Left joystick");
	that::Logger::Log("Shoot pump/pump to enemy/Breathe fire : South button (A on XBox)");
	that::Logger::Log("Become fygar ghost : Left shoulder");

}

int main(int, char* [])
{
	that::Engine engine("../Data/");
	engine.Run(Setup);

	return 0;
}