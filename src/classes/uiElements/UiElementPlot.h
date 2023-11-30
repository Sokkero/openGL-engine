#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    class UiElementPlot : public UiElement
    {
        public:
            explicit UiElementPlot(std::string text, std::vector<float> values = std::vector<float>())
                : m_text(std::move(text))
                , m_values(std::move(values)) {};
            ~UiElementPlot() = default;

            void drawUi() override
            {
                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }

                ImGui::PlotLines(m_text.c_str(), m_values.data(), m_values.size());
            }

            std::string getText() const { return m_text; };

            void setText(std::string text) { m_text = std::move(text); };

            void addValue(float value) { m_values.emplace_back(value); };

            void clearValues() { m_values.clear(); };

            std::vector<float> getValues() { return m_values; };

            void setValues(std::vector<float> values) { m_values = std::move(values); };

        private:
            std::vector<float> m_values;
            std::string m_text;
    };
} // namespace Engine::Ui