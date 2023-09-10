#include "EngineTypesReflection.h"

#include "PhysicsData.h"
#include "Reflection.h"

void EngineTypeReflection::InitEngineTypes()
{
	that::reflection::Reflection::RegisterBasicTypeAs<that::PhysicsMaterial::PropertyMixer, unsigned int>();
	that::reflection::Reflection::RegisterBasicTypeWithUnderlying
		<that::PhysicsMaterial, float, float, that::PhysicsMaterial::PropertyMixer, that::PhysicsMaterial::PropertyMixer>
		({ "Friction", "Elasticity", "Friction Mixer", "Elasticity Mixer" }
	);
}
