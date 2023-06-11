#include "GameData.h"

#include "Logger.h"

#include <fstream>
#include <filesystem>

int digdug::GameData::GetHighScore() const
{    
    // Find the biggest value
    auto biggest = m_HighScores.begin();
    for (auto it = m_HighScores.begin(); it != m_HighScores.end(); ++it) {
        if (it->first > biggest->first) {
            biggest = it;
        }
    }

    return biggest->first;
}

const std::string& digdug::GameData::GetHighScoreUser() const
{
    // Find the biggest value
    auto biggest = m_HighScores.begin();
    for (auto it = m_HighScores.begin(); it != m_HighScores.end(); ++it) {
        if (it->first > biggest->first) {
            biggest = it;
        }
    }

    return biggest->second;
}

int digdug::GameData::GetRoundNumber() const
{
    return m_RoundNumber;
}

void digdug::GameData::IncrementRoundNumber()
{
    ++m_RoundNumber;
}

void digdug::GameData::ResetGame()
{
    m_RoundNumber = 1;
    m_CurrentScores.clear();
}

bool digdug::GameData::TryNewHighScore(int score) const
{
    // Find the iterator pointing to the lowest value
    auto lowestIterator = std::min_element(
        m_HighScores.begin(), m_HighScores.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    );

    return score > lowestIterator->first;
}

void digdug::GameData::ApplyHighScore(int score, std::string user)
{
    m_HighScores.insert(std::make_pair(score, user));

    // Find the smallest value
    auto smallest = m_HighScores.begin();
    for (auto it = m_HighScores.begin(); it != m_HighScores.end(); ++it) {
        if (it->first < smallest->first) {
            smallest = it;
        }
    }

    m_HighScores.erase(smallest);
}

const std::multimap<int, std::string> digdug::GameData::GetHighScores() const
{
    return m_HighScores;
}

void digdug::GameData::SetCurrentScore(int index, int score)
{
    if (static_cast<int>(m_CurrentScores.size()) <= index)
    {
        m_CurrentScores.resize(index + 1);
    }

    m_CurrentScores[index] = score;
}

int digdug::GameData::GetCurrentScores(int index) const
{
    if (static_cast<int>(m_CurrentScores.size()) <= index) return 0;

    return m_CurrentScores[index];
}

digdug::GameData::GameData()
{
    // Write to file
    if (std::ifstream binaryIn{ "../Data/Saves/highscore.that", std::ios::binary }; binaryIn.is_open())
    {
        // For each high score
        for (int i{}; i < m_NrHighScores; ++i)
        {
            int highScore{};
            std::stringstream highScoreUser{};

            // Read high score
            binaryIn.read(reinterpret_cast<char*>(&highScore), sizeof(int));

            // Read high score user
            for (int charIdx{}; charIdx < m_NameSize; ++charIdx)
            {
                // Get the amount of data
                char newChar{};
                binaryIn.read(&newChar, sizeof(char));

                highScoreUser << newChar;
            }

            m_HighScores.insert(std::make_pair(highScore, highScoreUser.str()));
        }
    }
    else
    {
        for (int i{}; i < m_NrHighScores; ++i)
        {
            m_HighScores.insert({ 0, "---" });
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
        // For each high score
        for (const auto& highScorePair : m_HighScores)
        {
            // Write high score
            binaryOut.write(reinterpret_cast<const char*>(&highScorePair.first), sizeof(int));

            // Write high score user
            for (char c : highScorePair.second)
            {
                binaryOut.put(c);
            }
        }
    }
}
