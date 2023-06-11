#pragma once

#include "Scene.h"

#include <memory>
#include <string>

namespace that
{
	class GameObject;
}

namespace digdug
{
	class HighScoreTable final
	{
	public:
		static void Load(that::Scene& scene);
	private:
		static that::GameObject* CreateText(that::Scene& scene, const std::string& objectName, const std::string & text);
	};
}
