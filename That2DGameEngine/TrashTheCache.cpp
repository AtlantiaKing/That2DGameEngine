#include "TrashTheCache.h"
#include "imgui.h"
#include <imgui_plot.h>
#include <algorithm>
#include <chrono>
#include <numeric>

void that::TrashTheCache::Render()
{
    RenderExercise1();

    RenderExercise2();
}

void that::TrashTheCache::RenderExercise1()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 20, main_viewport->WorkPos.y + 20));
    ImGui::SetNextWindowSize(ImVec2(280, 400));

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Exercise 1"))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    constexpr ImVec2 buttonSize{ 50, 50 };

    ImGui::InputInt("samples", &m_Exercise1AmountSamples);

    if (ImGui::Button("Trash the cache"))
    {
        CalculateExercise1();
    }

    if (m_Exercise1Samples.empty()) return;

    ImGui::PlotConfig::Values plotValues{ nullptr, m_Exercise1Samples.data(), static_cast<int>(m_Exercise1Samples.size()), 0, ImColor{ 1.0f, 0.5f, 1.0f } };

    const float maxElement{ *std::max_element(begin(m_Exercise1Samples), end(m_Exercise1Samples)) };

    ImGui::PlotConfig plot{};
    plot.frame_size = ImVec2{ 200, 100 };
    plot.values = plotValues;
    plot.scale = ImGui::PlotConfig::Scale{ 0, maxElement };

    const size_t verticaIndices{ 4 };
    plot.v_lines = ImGui::PlotConfig::VerticalLines{ true, &verticaIndices, 1 };

    plot.grid_y = ImGui::PlotConfig::Grid{ true, maxElement / 10 };
    
    ImGui::Plot("exercise1", plot);
}

void that::TrashTheCache::CalculateExercise1()
{
    m_Exercise1Samples.clear();

    constexpr int bufferSize{ 100'000'000 };
    int* buffer{ new int[bufferSize] {} };

    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        std::vector<float> stepTimers{};

        for (int sample{}; sample < m_Exercise1AmountSamples; ++sample)
        {
            const auto start{ std::chrono::high_resolution_clock::now() };

            for (int i{}; i < bufferSize; i += stepSize)
            {
                buffer[i] *= 2;
            }

            const auto end{ std::chrono::high_resolution_clock::now() };
            const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };

            stepTimers.push_back(static_cast<float>(elapsedTime));
        }

        std::sort(begin(stepTimers), end(stepTimers));

        const float average{ std::accumulate(begin(stepTimers) + 1, end(stepTimers) - 1, 0.0f) / (stepTimers.size() - 2) };

        m_Exercise1Samples.push_back(average);
    }

    delete[] buffer;
}

void that::TrashTheCache::RenderExercise2()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 320, main_viewport->WorkPos.y + 20));
    ImGui::SetNextWindowSize(ImVec2(300, 400));

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Exercise 2"))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    {
        const float values[5]{ 1,2,3,4,5 };
        // TODO: Add cache timings
        ImGui::PlotConfig::Values plotValues{ nullptr, values, sizeof(values) / sizeof(float) };

        ImGui::PlotConfig plot{};
        plot.frame_size = ImVec2{ 100, 100 };
        plot.values = plotValues;
        plot.scale = ImGui::PlotConfig::Scale{ 0, 6 };

        ImGui::Plot("exercise2base", plot);
    }

    {
        const float values0[5]{ 1,2,3,4,5 };
        const float values1[5]{ 5,4,3,2,1 };
        const float* values[2]{ values0, values1 };
        // TODO: Add cache timings
        ImGui::PlotConfig::Values plotValues0{ nullptr, nullptr,  sizeof(values0) / sizeof(float), 0, 0, values, sizeof(values) / sizeof(float*) };

        ImGui::PlotConfig plot{};
        plot.frame_size = ImVec2{ 100, 100 };
        plot.values = plotValues0;
        plot.scale = ImGui::PlotConfig::Scale{ 0, 6 };

        ImGui::Plot("exercise2compare", plot);
    }
}
