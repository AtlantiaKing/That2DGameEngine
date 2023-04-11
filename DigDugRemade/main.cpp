#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "SteamAchievements.h"
#include "Achievements.h"

//#include "DemoScene.h"
#include "GameScene.h"

#include <vector>

void Setup()
{
	// Add the demo scene
	that::SceneManager::GetInstance().AddScene(digdug::LoadGameScene);
}

int main(int, char* [])
{
	std::vector<that::Achievement_t> achievements
	{
		that::Achievement_t{ static_cast<int>(digdug::Achievements::ACH_WIN_ONE_GAME), "ACH_WIN_ONE_GAME" }
	};

	that::Engine engine("../Data/");
	engine.Run(Setup, achievements);

	return 0;
}