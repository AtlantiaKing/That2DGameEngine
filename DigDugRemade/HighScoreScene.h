#pragma once

#include "Scene.h"

#include <memory>

namespace that
{
	class GameObject;
	class Font;
}

namespace digdug
{
	class HighScoreLetter;

	class HighScoreScene final
	{
	public:
		static void Load(that::Scene& scene);
	private:
		static HighScoreLetter* CreateLetterEditor(that::GameObject* pParent, std::shared_ptr<that::Font> pFont, int index);
	};
}
