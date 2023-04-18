#pragma once

#include "Component.h"
#include "Subject.h"

namespace digdug
{
	class ScoreComponent final : public that::Component
	{
	public:
		ScoreComponent() = default;
		virtual ~ScoreComponent() = default;

		void AddScore(int score);
		void RemoveScore(int score);

		int GetScore() const { return m_Score; }

		that::Subject<ScoreComponent>& OnScoreChange() { return m_ScoreChangedListeners; }
	private:
		that::Subject<ScoreComponent> m_ScoreChangedListeners{};
		int m_Score{};
	};
}

