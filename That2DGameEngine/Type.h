#pragma once

#include <functional>
#include <string>

#include "GameObject.h"

namespace that
{
	class Component;
	class Transform;
}

namespace that::reflection
{
	struct SerializableVariable final
	{
		template<typename T>
		inline static SerializableVariable Create(const char* varName, size_t offset)
		{
			SerializableVariable var{};
			var.name = varName;
			var.offset = offset;
			var.size = sizeof(T);
			return var;
		}

		std::string name{};
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
			t.prefab = [](that::GameObject* pGameObject) 
			{ 
				if constexpr (std::is_same<Transform, T>())
				{
					return pGameObject->GetTransform();
				}
				else
				{
					return pGameObject->AddComponent<T>();
				}
			};
			return t;
		}

		template<typename T>
		inline static size_t GetHash()
		{
			return std::hash<std::string>{}(typeid(T).name());
		}

		that::Component* Clone(that::GameObject* pGameobject) const { return prefab(pGameobject); }

		std::string name{};
		size_t hash{};
		size_t size{};
		std::function<that::Component*(that::GameObject*)> prefab{};
		std::vector<SerializableVariable> variables{};
	};
}