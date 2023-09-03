#pragma once

#include "Reflection.h"

#define REGISTER_CLASS(NAME, VARNAME) inline static that::reflection::TypeRegister<NAME> g_Registrated##VARNAME{};

#define REGISTER_VARIABLE(VARIABLE, VARIABLENAME, TYPE, OFFSET) inline static that::reflection::VarRegister<decltype(VARIABLE), TYPE> g_Registrated##TYPE##VARIABLENAME{ #VARIABLENAME, OFFSET };

namespace that::reflection
{
	template<typename T>
	class TypeRegister final
	{
	public:
		TypeRegister()
		{
			Reflection::RegisterClass<T>();
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
