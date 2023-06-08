#include "GameData.h"

int digdug::GameData::GetHighScore()
{
    return m_HighScore;
}

const std::string& digdug::GameData::GetHighScoreUser()
{
    return m_HighScoreUser;
}

int digdug::GameData::GetRoundNumber()
{
    return m_RoundNumber;
}

void digdug::GameData::ResetGame()
{
    m_RoundNumber = 1;
}

void digdug::GameData::TryNewHighScore(int score)
{
    m_HighScore = score > m_HighScore ? score : m_HighScore;
}

digdug::GameData::GameData()
{
    // LOAD HIGH SCORE DATA FROM FILE
}