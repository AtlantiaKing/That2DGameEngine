#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define EDITOR

#ifdef EDITOR
#include "Editor.h"
#else
#include "Engine.h"
#endif

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
	constexpr const char* dataPath{ "../Data/" };
#ifdef EDITOR
	that::Editor editor{ dataPath };
#else
	that::Engine engine{ dataPath };
	engine.Run(Setup);
#endif

	return 0;
}