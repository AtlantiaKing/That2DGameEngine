#pragma once

#include "Component.h"
#include "Observer.h"
#include "ScoreComponent.h"

namespace digdug
{
	class HighScoreHUDComponent final : public that::Component, public that::Observer<ScoreComponent>
	{
	public:
		HighScoreHUDComponent() = default;
		virtual ~HighScoreHUDComponent() = default;

		HighScoreHUDComponent(const HighScoreHUDComponent& other) = delete;
		HighScoreHUDComponent(HighScoreHUDComponent&& other) = delete;
		HighScoreHUDComponent& operator=(const HighScoreHUDComponent& other) = delete;
		HighScoreHUDComponent& operator=(HighScoreHUDComponent&& other) = delete;

		void SearchPlayer(that::GameObject* pPlayerParent);

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnDestroy() override;
		virtual void OnSubjectDestroy() override;

		virtual void Notify(const ScoreComponent& score) override;
	private:
		void Display(that::GameObject* pPlayer);

		int m_HighScore{};

		that::GameObject* m_pParent{};
		digdug::ScoreComponent* m_pScore{};
	};
}
