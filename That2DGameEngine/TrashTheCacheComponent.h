#pragma once

#include "Component.h"

#include <vector>

namespace that
{
	class TrashTheCacheComponent final : public Component
	{
	public:
		TrashTheCacheComponent();
		virtual ~TrashTheCacheComponent() = default;

		TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
		TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;

		virtual void OnGUI() override;
	private:
		struct Transform
		{
			float matrix[16]
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		struct GameObject3D
		{
			Transform transform{};
			int id{};
		};
		struct GameObject3DAlt
		{
			Transform* transform{};
			int id{};
		};

		void RenderPlot(const std::vector<float>& samples, size_t verticalLineIndex);
		void RenderDoublePlot();

		void RenderExercise1();
		void RenderExercise2();

		void CalculateExercise1();
		void CalculateExercise2Normal();
		void CalculateExercise2Alt();

		int m_Exercise1AmountSamples{ 10 };
		std::vector<float> m_Exercise1Samples{};

		int m_Exercise2AmountSamples{ 10 };
		std::vector<float> m_Exercise2SamplesNormal{};
		std::vector<float> m_Exercise2SamplesAlt{};
	};
}
