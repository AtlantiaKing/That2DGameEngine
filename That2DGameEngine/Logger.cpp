#include "Logger.h"
#include <iostream>
#include <cassert>

void that::Logger::Log(const std::string& message)
{
	std::cout << message << "\n\033[0;39m";
}

void that::Logger::LogWarning(const std::string& message)
{
	std::cout << "\033[0;33m" << message << "\n\033[0;39m";
}

void that::Logger::LogError(const std::string& message)
{
	std::cout << "\033[0;31m" << message << "\n\033[0;39m";
	throw LogException{};
}
