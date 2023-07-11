#pragma once

#include "Component.h"

namespace that
{
	class UIComponent;
}

namespace digdug
{
	class HighScoreButton final : public that::Component
	{
	public:
		HighScoreButton() = default;
		virtual ~HighScoreButton() = default;

		HighScoreButton(const HighScoreButton& other) = delete;
		HighScoreButton(HighScoreButton&& other) = delete;
		HighScoreButton& operator=(const HighScoreButton& other) = delete;
		HighScoreButton& operator=(HighScoreButton&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		float m_BlinkTime{};
		const float m_TimeBetweenBlinks{ 0.1f };

		that::UIComponent* m_pRenderer{};
	};
}
