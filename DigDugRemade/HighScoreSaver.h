#pragma once

#include "Component.h"

#include <vector>

namespace digdug
{
	class HighScoreLetter;

	class HighScoreSaver final : public that::Component
	{
	public:
		HighScoreSaver() = default;
		virtual ~HighScoreSaver() = default;

		HighScoreSaver(const HighScoreSaver& other) = delete;
		HighScoreSaver(HighScoreSaver&& other) = delete;
		HighScoreSaver& operator=(const HighScoreSaver& other) = delete;
		HighScoreSaver& operator=(HighScoreSaver&& other) = delete;

		void SetLetters(const std::vector<HighScoreLetter*>& pLetters);

		virtual void OnDestroy() override;

	private:
		std::vector<HighScoreLetter*> m_pLetters{};
	};
}

