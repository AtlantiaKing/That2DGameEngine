#pragma once

#include "Component.h"

#include <vector>

namespace digdug
{
	class HighScoreLetter;
	class HighScoreButton;

	class HighScoreInput final : public that::Component
	{
	public:
		HighScoreInput() = default;
		virtual ~HighScoreInput() = default;

		HighScoreInput(const HighScoreInput& other) = delete;
		HighScoreInput(HighScoreInput&& other) = delete;
		HighScoreInput& operator=(const HighScoreInput& other) = delete;
		HighScoreInput& operator=(HighScoreInput&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;

		void InputUp();
		void InputDown();

		void SetLetters(const std::vector<HighScoreLetter*>& pLetters);
		void SetDoneButton(HighScoreButton* pButton);

	private:
		std::vector<HighScoreLetter*> m_pLetters{};
		HighScoreButton* m_pCompleteButton{};

		int m_CurLetter{};
	};
}
