#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "DemoScene.h"

void Setup()
{
	// Add the demo scene
	that::SceneManager::GetInstance().AddScene(that::LoadDemoScene);
}

int main(int, char* [])
{
	that::Engine engine("../Data/");
	engine.Run(Setup);

	return 0;
}