#pragma once
class EngineTypeReflection final
{
public:
	static void InitEngineTypes();

private:
	struct EngineTypeInitializer final
	{
	public:
		EngineTypeInitializer()
		{
			InitEngineTypes();
		}
	};
	inline static EngineTypeInitializer initializer{};
};

