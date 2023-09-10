#include "Reflection.h"

#include "glm/vec2.hpp"

void that::reflection::Reflection::RegisterBasicTypes()
{
	m_BasicTypes.push_back(SerializedType::Create<int>());
	m_BasicTypes.push_back(SerializedType::Create<unsigned int>());
	m_BasicTypes.push_back(SerializedType::Create<float>());
	m_BasicTypes.push_back(SerializedType::Create<double>());
	m_BasicTypes.push_back(SerializedType::Create<bool>());
	m_BasicTypes.push_back(SerializedType::Create<char>());
	m_BasicTypes.push_back(SerializedType::CreateWithUnderlying<glm::vec2, float, float>(m_BasicTypes, { "x", "y" }));
}
