#pragma once
#include "Achievement.h"

#include <string>
#include <functional>
#include <vector>

namespace that
{
	class Engine
	{
	public:
		explicit Engine(const std::string& dataPath);
		~Engine();
		void Run(const std::function<void()>& load);

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
	};
}