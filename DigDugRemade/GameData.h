#pragma once

#include "Singleton.h"
#include <string>

namespace digdug
{
	class GameData final : public that::Singleton<GameData>
	{
	public:
		int GetHighScore();
		const std::string& GetHighScoreUser();
		int GetRoundNumber();

		void ResetGame();
		void TryNewHighScore(int score);
	private:
		friend class Singleton<GameData>;
		GameData();

		int m_HighScore{ 0 };
		std::string m_HighScoreUser{ "---" };

		int m_RoundNumber{ 1 };
	};
}

