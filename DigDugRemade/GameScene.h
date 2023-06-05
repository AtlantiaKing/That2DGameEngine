#pragma once

#include "Scene.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GameScene final
	{
	public:
		static void Load(that::Scene& scene);
	private:
		static that::GameObject* CreatePlayer(that::GameObject* pGrid);
		static void PrintControls();
	};
}