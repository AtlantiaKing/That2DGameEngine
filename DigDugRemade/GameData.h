#pragma once

#include "Singleton.h"
#include <string>

namespace digdug
{
	class GameData final : public that::Singleton<GameData>
	{
	public:
		int GetHighScore();
		std::string& GetHighScoreUser();
		int GetRoundNumber();

		void ResetGame();
		bool TryNewHighScore(int score);

		int GetMainMenuScene() { return m_MainMenuScene; }
		int GetHighScoreScene() { return m_HighScoreScene; }
	private:
		friend class Singleton<GameData>;
		GameData();
		~GameData();

		int m_HighScore{ 0 };
		std::string m_HighScoreUser{ "---" };

		const int m_MainMenuScene{ 0 };
		const int m_HighScoreScene{ 2 };

		int m_RoundNumber{ 1 };
	};
}

