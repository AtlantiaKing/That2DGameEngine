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

void that::TrashTheCache::RenderPlot(const std::vector<float>& samples, size_t verticalLineIndex)
{
    ImGui::PlotConfig::Values plotValues{ nullptr, samples.data(), static_cast<int>(samples.size()), 0, ImColor{ 1.0f, 0.5f, 1.0f } };

    const float maxElement{ *std::max_element(begin(samples), end(samples)) };

    ImGui::PlotConfig plot{};
    plot.frame_size = ImVec2{ 200, 100 };
    plot.values = plotValues;
    plot.scale = ImGui::PlotConfig::Scale{ 0, maxElement };

    plot.v_lines = ImGui::PlotConfig::VerticalLines{ true, &verticalLineIndex, 1 };

    plot.grid_y = ImGui::PlotConfig::Grid{ true, maxElement / 10 };

    ImGui::Plot("plotter", plot);
}

void that::TrashTheCache::RenderDoublePlot()
{
    const float* doubleValues[2]{ m_Exercise2SamplesNormal.data(), m_Exercise2SamplesAlt.data() };
    ImU32 colors[2]{ ImColor{ 1.0f, 0.0f, 0.0f }, ImColor{ 0.0f, 1.0f, 0.0f } };

    ImGui::PlotConfig::Values plotValues{ nullptr, nullptr, static_cast<int>(m_Exercise2SamplesNormal.size()), 0, 0, doubleValues, 2, colors };

    const float maxElement{ *std::max_element(begin(m_Exercise2SamplesNormal), end(m_Exercise2SamplesNormal)) };

    ImGui::PlotConfig plot{};
    plot.frame_size = ImVec2{ 200, 100 };
    plot.values = plotValues;
    plot.scale = ImGui::PlotConfig::Scale{ 0, maxElement };

    plot.grid_y = ImGui::PlotConfig::Grid{ true, maxElement / 10 };

    ImGui::Plot("doubleplotter", plot);
}

void that::TrashTheCache::RenderExercise1()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 20, main_viewport->WorkPos.y + 20));
    ImGui::SetNextWindowSize(ImVec2(280, 450));

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Exercise 1"))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    ImGui::InputInt("samples", &m_Exercise1AmountSamples);

    if (ImGui::Button("Trash the cache"))
    {
        CalculateExercise1();
    }

    if (!m_Exercise1Samples.empty())
    {
        RenderPlot(m_Exercise1Samples, 4);
    }

    ImGui::End();
}

void that::TrashTheCache::RenderExercise2()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 320, main_viewport->WorkPos.y + 20));
    ImGui::SetNextWindowSize(ImVec2(300, 450));

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Exercise 2"))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    ImGui::InputInt("samples", &m_Exercise2AmountSamples);

    if (ImGui::Button("Trash the cache with GameObject3D"))
    {
        CalculateExercise2Normal();
    }

    if (!m_Exercise2SamplesNormal.empty())
    {
        RenderPlot(m_Exercise2SamplesNormal, 3);
    }

    if (ImGui::Button("Trash the cache with GameObject3DAlt"))
    {
        CalculateExercise2Alt();
    }

    if (!m_Exercise2SamplesAlt.empty())
    {
        RenderPlot(m_Exercise2SamplesAlt, 3);
    }

    if (!m_Exercise2SamplesNormal.empty() && !m_Exercise2SamplesAlt.empty())
    {
        RenderDoublePlot();
    }

    ImGui::End();
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

void that::TrashTheCache::CalculateExercise2Normal()
{
    m_Exercise2SamplesNormal.clear();

    constexpr int bufferSize{ 10'000'000 };
    GameObject3D* buffer{ new GameObject3D[bufferSize] {} };

    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        std::vector<float> stepTimers{};

        for (int sample{}; sample < m_Exercise2AmountSamples; ++sample)
        {
            const auto start{ std::chrono::high_resolution_clock::now() };

            for (int i{}; i < bufferSize; i += stepSize)
            {
                buffer[i].id *= 2;
            }

            const auto end{ std::chrono::high_resolution_clock::now() };
            const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };

            stepTimers.push_back(static_cast<float>(elapsedTime));
        }

        std::sort(begin(stepTimers), end(stepTimers));

        const float average{ std::accumulate(begin(stepTimers) + 1, end(stepTimers) - 1, 0.0f) / (stepTimers.size() - 2) };

        m_Exercise2SamplesNormal.push_back(average);
    }

    delete[] buffer;
}

void that::TrashTheCache::CalculateExercise2Alt()
{
    m_Exercise2SamplesAlt.clear();

    constexpr int bufferSize{ 10'000'000 };
    GameObject3DAlt* buffer{ new GameObject3DAlt[bufferSize] {} };

    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        std::vector<float> stepTimers{};

        for (int sample{}; sample < m_Exercise2AmountSamples; ++sample)
        {
            const auto start{ std::chrono::high_resolution_clock::now() };

            for (int i{}; i < bufferSize; i += stepSize)
            {
                buffer[i].id *= 2;
            }

            const auto end{ std::chrono::high_resolution_clock::now() };
            const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };

            stepTimers.push_back(static_cast<float>(elapsedTime));
        }

        std::sort(begin(stepTimers), end(stepTimers));

        const float average{ std::accumulate(begin(stepTimers) + 1, end(stepTimers) - 1, 0.0f) / (stepTimers.size() - 2) };

        m_Exercise2SamplesAlt.push_back(average);
    }

    delete[] buffer;
}
