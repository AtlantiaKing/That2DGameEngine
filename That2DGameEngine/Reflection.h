#pragma once

#include "Type.h"
#include <vector>
#include <fstream>
#include <iostream>

namespace that::reflection
{
	class Reflection final
	{
	public:
		template<typename T>
		inline static void RegisterClass()
		{
			m_Types.push_back(SerializableComponent::Create<T>());
		}

		inline static void RegisterClass(const SerializableComponent& sc)
		{
			m_Types.push_back(sc);
		}

		template<typename VarType, typename ClassType>
		inline static void RegisterVariable(const char* varName, size_t offset)
		{
			// Get the component to which this variable belongs
			SerializableComponent& comp{ GetType<ClassType>() };

			// Add a serialized variable to this component
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
