#pragma once

#include "Reflection.h"
#include "Type.h"

#define REGISTER_VARIABLE(VARIABLE, VARIABLENAME, TYPE, OFFSET) inline static that::reflection::VarRegister<decltype(VARIABLE), TYPE> g_Registrated##TYPE##VARIABLENAME{ #VARIABLENAME, OFFSET };

namespace that::reflection
{
	template<typename T>
	class TypeRegister final
	{
	public:
		TypeRegister()
		{
			SerializableComponent sc{};
			sc.name = typeid(T).name();
			sc.hash = SerializableComponent::GetHash<T>();
			sc.size = sizeof(T);
			sc.prefab = [](that::GameObject* pGameObject) -> that::Component*
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

			Reflection::RegisterClass(sc);
		}
	};

	template<typename VarType, typename ClassType>
	class VarRegister final
	{
	public:
		VarRegister(const char* varName, size_t offset)
		{
			Reflection::RegisterVariable<VarType, ClassType>(varName, offset);
		}
	};
}
