#pragma once

#include "UiElement.h"

#include "utils/MathUtils.h"

#include <implot.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Engine::Ui
{
    template<typename T>
    class UiElementPieChart : public UiElement
    {
        public:
            UiElementPieChart(std::string title, const int maxValues = 10)
                : m_maxValues(maxValues)
                , m_colorScheme(ImPlotColormap_Pastel)
                , m_chartTitle(std::move(title)) {};

            ~UiElementPieChart() = default;

            void drawUi() override
            {
                ImPlot::PushColormap(m_colorScheme);
                if(ImPlot::BeginPlot(m_chartTitle.c_str(), ImVec2(260, 300), ImPlotFlags_Equal | ImPlotFlags_NoMouseText))
                {
                    ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, 1, 0, 1);
                    ImPlot::PlotPieChart(
                            getLabelVector().data(),
                            getPercentageVector().data(),
                            (int)m_percentageMap.size(),
                            0.5,
                            0.5,
                            0.4,
                            "%.2f",
                            90,
                            ImPlotPieChartFlags_Normalize
                    );
                    ImPlot::EndPlot();
                }
                ImPlot::PopColormap();
            }

            void addValue(const char* label, T value)
            {
                std::vector<T>& values = m_valueMap[label];
                values.emplace_back(value);

                if(values.size() > m_maxValues)
                {
                    values.erase(values.begin());
                }

                m_averageMap[label] = MathUtils::GetAverage(values);

                float sum = 0;
                for(const auto& pair : m_averageMap)
                {
                    sum += (float)pair.second;
                }

                for(const auto& pair : m_averageMap)
                {
                    m_percentageMap[pair.first] = (float)pair.second / sum;
                }
            };

            void clearValues()
            {
                m_averageMap.clear();
                m_valueMap.clear();
            };

            void setMaxValues(int values) { m_maxValues = values; };

            void setColorSchema(ImPlotColormap_ schema) { m_colorScheme = schema; };

        private:
            std::vector<const char*> getLabelVector()
            {
                std::vector<const char*> labels;
                for(const auto& pair : m_valueMap)
                {
                    labels.emplace_back(pair.first);
                }
                return labels;
            }

            std::vector<float> getPercentageVector()
            {
                std::vector<float> percentages;
                for(const auto& pair : m_percentageMap)
                {
                    percentages.emplace_back(pair.second);
                }
                return percentages;
            }

            ImPlotColormap_ m_colorScheme;
            std::map<const char*, std::vector<T>> m_valueMap;
            std::map<const char*, T> m_averageMap;
            std::map<const char*, float> m_percentageMap;
            std::string m_chartTitle;
            int m_maxValues;
    };
} // namespace Engine::Ui