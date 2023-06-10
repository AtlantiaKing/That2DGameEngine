#include "HighScoreSaver.h"

#include "GameData.h"

#include "HighScoreLetter.h"

#include <sstream>

void digdug::HighScoreSaver::SetLetters(const std::vector<HighScoreLetter*>& pLetters)
{
	m_pLetters = pLetters;
}

void digdug::HighScoreSaver::OnDestroy()
{
	const int score{ GameData::GetInstance().GetCurrentScores(0) };

	std::stringstream user{};
	for (HighScoreLetter* pLetter : m_pLetters)
	{
		user << pLetter->GetLetter();
	}

	GameData::GetInstance().ApplyHighScore(score, user.str());
}
