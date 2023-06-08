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
#include "HighScoreScene.h"

#include <vector>

void Setup()
{
	that::Window::GetInstance().SetWindowSize(610, 520);
	that::SceneManager::GetInstance().AddScene(digdug::MainMenuScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::GameScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::HighScoreScene::Load);
}

int main(int, char* [])
{
	that::Engine engine("../Data/");
	engine.Run(Setup);

	return 0;
}