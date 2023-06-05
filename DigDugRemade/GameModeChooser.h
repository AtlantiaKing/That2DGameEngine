#pragma once

#include "Component.h"

#include "glm/vec2.hpp"

#include <vector>

namespace digdug
{
	class GameModeChooser final : public that::Component
	{
	public:
		GameModeChooser() = default;
		virtual ~GameModeChooser() = default;

		GameModeChooser(const GameModeChooser& other) = delete;
		GameModeChooser(GameModeChooser&& other) = delete;
		GameModeChooser& operator=(const GameModeChooser& other) = delete;
		GameModeChooser& operator=(GameModeChooser&& other) = delete;

		void SetMarkerPositions(const glm::vec2& marker1, const glm::vec2& marker2, const glm::vec2& marker3);

		void BindInput();
	private:
		void MarkerUp();
		void MarkerDown();
		void LoadScene();

		void UpdatePosition();

		std::vector<glm::vec2> m_Markers{};
		int m_CurMarker{};
	};
}