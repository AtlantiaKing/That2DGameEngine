#pragma once

#include <functional>
#include <string>
#include <vector>
#include <sstream>

#include "GameObject.h"

namespace that
{
	class Component;
	class Transform;
}

namespace that::reflection
{
	template<typename T>
	inline static size_t GetHash()
	{
		return std::hash<std::string>{}(typeid(T).name());
	}

	struct SerializableVariable final
	{
		template<typename T>
		inline static SerializableVariable Create(const char* varName, size_t offset)
		{
			SerializableVariable var{};
			var.name = varName;
			var.offset = offset;
			var.size = sizeof(T);
			var.hash = GetHash<T>();
			return var;
		}

		constexpr SerializableVariable() = default;

		std::string name{};
		size_t hash{};
		size_t size{};
		size_t offset{};
	};

	struct SerializableComponent final
	{
		template<typename T>
		inline static SerializableComponent Create()
		{
			SerializableComponent t{};
			t.name = typeid(T).name();
			t.hash = GetHash<T>();
			t.size = sizeof(T);
			t.prefab = [](that::GameObject* pGameObject) -> that::Component*
			{ 
				if constexpr (std::is_same<Transform, T>())
				{
					return reinterpret_cast<that::Component*>(pGameObject->GetTransform());
				}
				else
				{
					return reinterpret_cast<that::Component*>(pGameObject->AddComponent<T>());
				}
			};
			return t;
		}

		that::Component* Clone(that::GameObject* pGameobject) const { return prefab(pGameobject); }

		constexpr SerializableComponent() = default;

		std::string name{};
		size_t hash{};
		size_t size{};
		that::Component* (*prefab)(that::GameObject*) {};
		std::vector<SerializableVariable> variables{};
	};

	struct SerializedType final
	{
		template<typename T>
		inline static SerializedType Create()
		{
			SerializedType t{};
			t.name = typeid(T).name();
			t.hash = GetHash<T>();
			t.size = sizeof(T);
			t.dataToString = [](void* pData) -> std::string
			{
				T* pTypedData{ reinterpret_cast<T*>(pData) };
				std::stringstream ss{};
				ss << std::boolalpha << *pTypedData;
				return ss.str();
			};
			return t;
		}

		template<typename T, typename AsT>
		inline static SerializedType CreateAs()
		{
			SerializedType t{};
			t.name = typeid(T).name();
			t.hash = GetHash<T>();
			t.size = sizeof(T);
			t.dataToString = [](void* pData) -> std::string
			{
				AsT* pTypedData{ reinterpret_cast<AsT*>(pData) };
				std::stringstream ss{};
				ss << std::boolalpha << *pTypedData;
				return ss.str();
			};
			return t;
		}

		template<typename T, typename... Args>
		inline static SerializedType CreateWithUnderlying(const std::vector<SerializedType>& basicTypes, std::vector<std::string> names)
		{
			SerializedType t{};
			t.name = typeid(T).name();
			t.hash = GetHash<T>();
			t.size = sizeof(T);
			t.AddUnderlyingVars<Args...>(basicTypes, names);
			return t;
		}

		std::string name{};
		size_t hash{};
		size_t size{};
		std::string (*dataToString)(void*) {};
		std::vector<std::pair<std::string, SerializedType>> underlyingTypes{};

	private:
		template <typename T>
		static void AddUnderlyingVar(SerializedType* pType, const std::vector<SerializedType>& basicTypes, std::vector<std::string>& names)
		{
			const auto& typeIt{ std::find_if(begin(basicTypes), end(basicTypes), [](const auto& basicType) { return basicType.hash == GetHash<T>(); }) };

			if (typeIt == end(basicTypes)) return;

			pType->underlyingTypes.push_back(std::make_pair(names[0], *typeIt));

			names.erase(begin(names));
		}

		template<typename... Args>
		struct UnderlyingVarAdder;

		template <typename T, typename... Args>
		struct UnderlyingVarAdder<T, Args...>
		{
			static void AddUnderlyingVars(SerializedType* pType, const std::vector<SerializedType>& basicTypes, std::vector<std::string>& names)
			{
				AddUnderlyingVar<T>(pType, basicTypes, names);
				UnderlyingVarAdder<Args...>::AddUnderlyingVars(pType, basicTypes, names);
			}
		};

		template <>
		struct UnderlyingVarAdder<>
		{
			static void AddUnderlyingVars(SerializedType*, const std::vector<SerializedType>&, std::vector<std::string>&) {}
		};

		template<typename... Args>
		void AddUnderlyingVars(const std::vector<SerializedType>& basicTypes, std::vector<std::string>& names)
		{
			return UnderlyingVarAdder<Args...>::AddUnderlyingVars(this, basicTypes, names);
		}
	};
}