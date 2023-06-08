#include "GameData.h"

#include "Logger.h"

#include <fstream>
#include <filesystem>

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
    // Write to file
    if (std::ifstream binaryIn{ "../Data/Saves/highscore.that", std::ios::binary }; binaryIn.is_open())
    {
        // Read high score
        binaryIn.read(reinterpret_cast<char*>(&m_HighScore), sizeof(int));

        // Read high score user
        for (char& c : m_HighScoreUser)
        {
            // Get the amount of data
            char newChar{};
            binaryIn.read(&newChar, sizeof(char));

            c = newChar;
        }
    }
}

digdug::GameData::~GameData()
{
    std::string filePath = "../Data/Saves/highscore.that";

    std::filesystem::path directoryPath = std::filesystem::path(filePath).parent_path();
    if (!std::filesystem::exists(directoryPath))
    {
        if (std::filesystem::create_directories(directoryPath))
            that::Logger::Log("Save file directory created");
        else
            that::Logger::LogError("Failed to create save file directory");
    }

    // Write to file
    if (std::ofstream binaryOut{ filePath, std::ios::binary | std::ofstream::trunc }; binaryOut.is_open())
    {
        // Write high score
        binaryOut.write(reinterpret_cast<const char*>(&m_HighScore), sizeof(int));

        // Write high score user
        for (char c : m_HighScoreUser)
        {
            binaryOut.put(c);
        }
    }
}
