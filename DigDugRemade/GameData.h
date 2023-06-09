#pragma once

#include "Singleton.h"
#include <string>
#include <vector>

namespace digdug
{
	class GameData final : public that::Singleton<GameData>
	{
	public:
		int GetHighScore();
		std::string& GetHighScoreUser();
		int GetRoundNumber();
		void IncrementRoundNumber();

		void ResetGame();
		bool TryNewHighScore(int score);

		int GetMainMenuScene() { return m_MainMenuScene; }
		int GetHighScoreScene() { return m_HighScoreScene; }
		int GetSinglePlayerScene() { return m_SinglePlayerScene; }
		int GetVersusPlayerScene() { return m_VersusPlayerScene; }
		int GetCoOpPlayerScene() { return m_CoOpPlayerScene; }

		void SetCurrentScore(int index, int score);
		int GetCurrentScores(int index) const;
	private:
		friend class Singleton<GameData>;
		GameData();
		~GameData();

		int m_HighScore{ 0 };
		std::string m_HighScoreUser{ "---" };

		std::vector<int> m_CurrentScores{};

		const int m_MainMenuScene{ 0 };
		const int m_SinglePlayerScene{ 1 };
		const int m_VersusPlayerScene{ 2 };
		const int m_CoOpPlayerScene{ 3 };
		const int m_HighScoreScene{ 4 };

		int m_RoundNumber{ 1 };
	};
}

