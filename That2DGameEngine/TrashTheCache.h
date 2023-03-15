#pragma once
#include <vector>

namespace that
{
	class TrashTheCache
	{
	public:
		void Render();
		void RenderExercise1();
		void CalculateExercise1();
		void RenderExercise2();
	private:
		int m_Exercise1AmountSamples{ 10 };
		std::vector<float> m_Exercise1Samples{};
	};
}

