#pragma once

#include "glm/vec2.hpp"

namespace that
{
	class Scene;
}

namespace digdug
{
	void SpawnScore(that::Scene* pScene, const glm::vec2& position, int score);
}