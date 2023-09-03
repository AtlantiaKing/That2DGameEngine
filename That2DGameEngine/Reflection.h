#pragma once

#include "Type.h"
#include <vector>
#include <fstream>

namespace that::reflection
{
	class Reflection final
	{
	public:
		template<typename T>
		static void RegisterClass()
		{
			m_Types.push_back(SerializableComponent::Create<T>());
		}

		template<typename VarType, typename ClassType>
		static void RegisterVariable(const char* varName, size_t offset)
		{
			SerializableComponent& comp{ GetType<ClassType>() };
			comp.variables.push_back(SerializableVariable::Create<VarType>(varName, offset));
		}

		inline static const std::vector<SerializableComponent>& GetTypes()
		{
			return m_Types;
		}

		template<typename T>
		inline static SerializableComponent& GetType()
		{
			return *std::find_if(begin(m_Types), end(m_Types), [](const SerializableComponent& type) { return SerializableComponent::GetHash<T>() == type.hash; });
		}

		inline static SerializableComponent& GetType(size_t hash)
		{
			return *std::find_if(begin(m_Types), end(m_Types), [hash](const SerializableComponent& type) { return hash == type.hash; });
		}

	private:
		inline static std::vector<SerializableComponent> m_Types{};
	};
}
