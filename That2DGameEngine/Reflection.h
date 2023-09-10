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

		template<typename T>
		inline static void RegisterBasicType()
		{
			m_BasicTypes.push_back(SerializedType::Create<T>());
		}

		template<typename T, typename AsT>
		inline static void RegisterBasicTypeAs()
		{
			m_BasicTypes.push_back(SerializedType::CreateAs<T, AsT>());
		}

		template<typename T, typename... Args>
		inline static void RegisterBasicTypeWithUnderlying(const std::vector<std::string>& names)
		{
			m_BasicTypes.push_back(SerializedType::CreateWithUnderlying<T, Args...>(m_BasicTypes, names));
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

		inline static const std::vector<SerializedType>& GetBasicTypes()
		{
			return m_BasicTypes;
		}

		template<typename T>
		inline static SerializableComponent& GetType()
		{
			return *std::find_if(begin(m_Types), end(m_Types), [](const SerializableComponent& type) { return GetHash<T>() == type.hash; });
		}

		inline static SerializableComponent& GetType(size_t hash)
		{
			return *std::find_if(begin(m_Types), end(m_Types), [hash](const SerializableComponent& type) { return hash == type.hash; });
		}

	private:
		inline static std::vector<SerializableComponent> m_Types{};
		inline static std::vector<SerializedType> m_BasicTypes{};

		static void RegisterBasicTypes();

		struct InitBasicTypes final
		{
		public:
			InitBasicTypes()
			{
				Reflection::RegisterBasicTypes();
			}
		};
		inline static InitBasicTypes m_BasicTypesInitializer{};
	};
}
