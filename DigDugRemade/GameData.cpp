#include "GameData.h"

int digdug::GameData::GetHighScore()
{
    return m_HighScore;
}

std::string& digdug::GameData::GetHighScoreUser()
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

bool digdug::GameData::TryNewHighScore(int score)
{
    if (score > m_HighScore)
    {
        m_HighScore = score;
        return true;
    }

    return false;
}

digdug::GameData::GameData()
{
    // LOAD HIGH SCORE DATA FROM FILE
}