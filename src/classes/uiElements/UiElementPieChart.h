#pragma once

#include "../helper/MathUtils.h"
#include "UiElement.h"

#include <implot.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Engine::Ui
{
    class UiElementPieChart : public UiElement
    {
        public:
            UiElementPieChart(std::string title, const std::vector<const char*>& fieldLabels, const int maxValues = 10)
                : m_maxValues(maxValues)
                , m_colorScheme(ImPlotColormap_Pastel)
                , m_chartTitle(std::move(title))
                , m_labels(fieldLabels)
            {
                for(int i = 0; i < m_labels.size(); i++)
                {
                    m_averageValues.push_back(0);
                }
            };

            ~UiElementPieChart() = default;

            void drawUi() override
            {
                ImPlot::PushColormap(m_colorScheme);
                if(ImPlot::BeginPlot(m_chartTitle.c_str(), ImVec2(260, 300), ImPlotFlags_Equal | ImPlotFlags_NoMouseText))
                {
                    ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, 1, 0, 1);
                    ImPlot::PlotPieChart(
                            m_labels.data(),
                            m_averageValues.data(),
                            (int)m_labels.size(),
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

            void addValue(const char* label, double value)
            {
                std::vector<double>& values = m_valueMap[label];

                values.emplace_back(value);
                if(values.size() > m_maxValues)
                {
                    values.erase(values.begin());
                }

                const auto elemPos = find(m_labels.begin(), m_labels.end(), label) - m_labels.begin();
                if(elemPos < m_labels.size())
                {
                    m_averageValues[elemPos] = MathUtils::GetAverage(values);
                }
                else
                {
                    // Tried adding a value to a field that does not exist
                    assert(false);
                }
            };

            void clearValues()
            {
                assert(m_labels.size() == m_averageValues.size());

                for(int i = 0; i < m_labels.size(); i++)
                {
                    m_averageValues[i] = 0;
                    m_valueMap[m_labels[i]].clear();
                }
            };

            void setMaxValues(int values) { m_maxValues = values; };

            void setColorSchema(ImPlotColormap_ schema) { m_colorScheme = schema; };

        private:
            ImPlotColormap_ m_colorScheme;
            std::map<const char*, std::vector<double>> m_valueMap;
            std::vector<const char*> m_labels;
            std::vector<double> m_averageValues;
            std::string m_chartTitle;
            int m_maxValues;
    };
} // namespace Engine::Ui