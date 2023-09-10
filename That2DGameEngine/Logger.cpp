#include "Logger.h"
#include "GameObject.h"
#include <sstream>
#include <iostream>

bool that::Logger::m_ErrorShouldThrow{ true };

void that::Logger::Log(const std::string& message, const GameObject* pGameObject)
{
	std::stringstream stream{};
	if (pGameObject) stream << "MESSAGE IN: " << pGameObject->GetName() << " --- ";
	stream << message;

	std::cout << message << "\n\033[0;39m";
}

void that::Logger::LogWarning(const std::string& message, const GameObject* pGameObject)
{
	std::stringstream stream{};
	if (pGameObject) stream << "WARNING IN: " << pGameObject->GetName() << " --- ";
	stream << message;

	std::cout << "\033[0;33m" << stream.str() << "\n\033[0;39m";
}

void that::Logger::LogError(const std::string& message, const GameObject* pGameObject)
{
	std::stringstream stream{};
	if (pGameObject) stream << "ERROR IN: " << pGameObject->GetName() << " --- ";
	stream << message;

	std::cout << "\033[0;31m" << message << "\n\033[0;39m";
	if(m_ErrorShouldThrow) throw LogException{};
}

void that::Logger::Clear()
{
	std::cout.flush();
}

void that::Logger::ErrorShouldThrow(bool shouldErrorThrow)
{
	m_ErrorShouldThrow = shouldErrorThrow;
}
