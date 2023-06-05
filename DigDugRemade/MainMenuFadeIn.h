#pragma once

#include "Component.h"
#include "Subject.h"

namespace digdug
{
	class MainMenuFadeIn final : public that::Component
	{
	public:
		MainMenuFadeIn() = default;
		virtual ~MainMenuFadeIn() = default;

		MainMenuFadeIn(const MainMenuFadeIn& other) = delete;
		MainMenuFadeIn(MainMenuFadeIn&& other) = delete;
		MainMenuFadeIn& operator=(const MainMenuFadeIn& other) = delete;
		MainMenuFadeIn& operator=(MainMenuFadeIn&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;

		that::Subject<MainMenuFadeIn> OnFadeComplete;
	private:
		void EndFade();

		const float m_FadeSpeed{ 100.0f };
	};
}
