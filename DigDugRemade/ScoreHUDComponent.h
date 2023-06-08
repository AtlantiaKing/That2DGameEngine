#pragma once

#include "Component.h"
#include "Observer.h"
#include "ScoreComponent.h"

namespace digdug
{
	class ScoreHUDComponent final : public that::Component, public that::Observer<ScoreComponent>
	{
	public:
		ScoreHUDComponent() = default;
		virtual ~ScoreHUDComponent() = default;

		ScoreHUDComponent(const ScoreHUDComponent& other) = delete;
		ScoreHUDComponent(ScoreHUDComponent&& other) = delete;
		ScoreHUDComponent& operator=(const ScoreHUDComponent& other) = delete;
		ScoreHUDComponent& operator=(ScoreHUDComponent&& other) = delete;

		void SearchPlayer(that::GameObject* pPlayerParent);

		virtual void Update() override;
		virtual void OnDestroy() override;

		virtual void Notify(const ScoreComponent& score) override;
	private:
		void Display(that::GameObject* pPlayer);

		that::GameObject* m_pParent{};
		digdug::ScoreComponent* m_pScore{};
	};
}
