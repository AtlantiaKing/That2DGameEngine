#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "Window.h"

#include "TestScene.h"
#include "FileScene.h"
#include "Logger.h"

#include <vector>

void Setup()
{
	that::Window::GetInstance().SetTitle("Programming 4 assignment");
	that::Window::GetInstance().SetWindowSize(610, 520);

	that::SceneManager::GetInstance().AddScene(digdug::TestScene::Load);
	that::SceneManager::GetInstance().AddScene(digdug::FileScene::Load);
}

int main(int, char* [])
{
	that::Engine engine("../Data/");
	engine.Run(Setup);

	return 0;
}