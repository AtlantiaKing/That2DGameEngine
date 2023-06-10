#pragma once

#include "Singleton.h"
#include <string>
#include <vector>
#include <map>

namespace digdug
{
	class GameData final : public that::Singleton<GameData>
	{
	public:
		int GetHighScore() const;
		const std::string& GetHighScoreUser() const;
		int GetRoundNumber() const;
		void IncrementRoundNumber();

		void ResetGame();
		bool TryNewHighScore(int score) const;
		void ApplyHighScore(int score, std::string user);

		int GetMainMenuScene() const { return m_MainMenuScene; }
		int GetHighScoreScene() const { return m_HighScoreScene; }
		int GetSinglePlayerScene() const { return m_SinglePlayerScene; }
		int GetVersusPlayerScene() const { return m_VersusPlayerScene; }
		int GetCoOpPlayerScene() const { return m_CoOpPlayerScene; }

		void SetCurrentScore(int index, int score);
		int GetCurrentScores(int index) const;
	private:
		friend class Singleton<GameData>;
		GameData();
		~GameData();

		const int m_NrHighScores{ 10 };
		const int m_NameSize{ 3 };
		std::multimap<int, std::string> m_HighScores{};

		std::vector<int> m_CurrentScores{};

		const int m_MainMenuScene{ 0 };
		const int m_SinglePlayerScene{ 1 };
		const int m_VersusPlayerScene{ 2 };
		const int m_CoOpPlayerScene{ 3 };
		const int m_HighScoreScene{ 4 };

		int m_RoundNumber{ 1 };
	};
}

