#pragma once

#include "UiElement.h"

#include <string>
#include <utility>

namespace Engine::Ui
{
    /**
     * @brief Represents a plot UI element.
     */
    class UiElementPlot : public UiElement
    {
        public:
            /**
             * @brief Constructs a UiElementPlot object with the given text and values.
             * @param text The text to be displayed on the plot.
             * @param values The values to be plotted.
             */
            explicit UiElementPlot(std::string text, std::vector<float> values = std::vector<float>())
                : m_text(std::move(text))
                , m_values(std::move(values))
                , m_maxValues(50) {};
            ~UiElementPlot() = default;

            /**
             * @brief Draws the plot UI element.
             * Overrides the drawUi() function of the base class.
             */
            void drawUi() override
            {
                if(isSameLine())
                {
                    ImGui::SameLine(getXOffset(), getSpacing());
                }

                ImGui::PlotLines(m_text.c_str(), m_values.data(), m_values.size());
            }

            /**
             * @brief Gets the text of the plot.
             * @return The text of the plot.
             */
            std::string getText() const { return m_text; };

            /**
             * @brief Sets the text of the plot.
             * @param text The text to be set.
             */
            void setText(std::string text) { m_text = std::move(text); };

            /**
             * @brief Adds a value to the plot.
             * @param value The value to be added.
             */
            void addValue(float value)
            {
                m_values.emplace_back(value);
                if(m_values.size() > m_maxValues)
                {
                    m_values.erase(m_values.begin());
                }
            };

            /**
             * @brief Clears all the values of the plot.
             */
            void clearValues() { m_values.clear(); };

            /**
             * @brief Gets the values of the plot.
             * @return The values of the plot.
             */
            std::vector<float> getValues() { return m_values; };

            /**
             * @brief Sets the values of the plot.
             * @param values The values to be set.
             */
            void setValues(std::vector<float> values) { m_values = std::move(values); };

            /**
             * @brief Gets the max amount of values of the plot.
             * @return The max amount of values of the plot.
             */
            int getMaxValues() const { return m_maxValues; };

            /**
             * @brief Sets the max amount of values of the plot.
             * @param values The max amount of values to be set.
             */
            void setMaxValues(int values) { m_maxValues = values; };

        private:
            std::vector<float> m_values;
            std::string m_text;
            int m_maxValues;
    };
} // namespace Engine::Ui