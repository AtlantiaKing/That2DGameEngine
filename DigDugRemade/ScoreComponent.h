#pragma once

#include "Component.h"
#include "EventListener.h"
#include "Events.h"

#include "Subject.h"

namespace digdug
{
	class ScoreComponent final : public that::Component, public that::EventListener<that::EntityDeathEvent>
	{
	public:
		ScoreComponent() = default;
		virtual ~ScoreComponent() = default;

		void AddScore(int score);
		void RemoveScore(int score);

		int GetScore() const { return m_Score; }

		virtual void Init() override;
		virtual void OnDestroy() override;
		virtual void OnEvent(that::EntityDeathEvent* e) override;

		that::Subject<ScoreComponent> OnScoreChange{};
	private:
		int GetScoreForLayer(int height) const;

		int m_Score{};

		const float m_FygarHorizontalEpsilon{ 8.0f };
	};
}

