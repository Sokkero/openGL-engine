#pragma once

#include "../../classes/nodeComponents/BasicNode.h"
#include "FieldTile.h"

namespace Engine
{
    class EngineManager;
}

class WafeFunctionCollapseSceneOrigin : public Engine::BasicNode
{
    public:
        WafeFunctionCollapseSceneOrigin(const glm::ivec2& fieldDimension);
        ~WafeFunctionCollapseSceneOrigin() = default;

    private:
        // Performance measuring
        static void printHumanReadable(double time, std::string text = "")
        {
            const int hours = static_cast<int>(time) / 3600;
            const int minutes = (static_cast<int>(time) % 3600) / 60;
            const int seconds = static_cast<int>(time) % 60;
            const int milliseconds = static_cast<int>((time - static_cast<int>(time)) * 1000);
            const int nanoseconds = static_cast<int>((time - static_cast<int>(time)) * 1e9) % 1000;

            std::cout << text
                      << hours << "h "
                      << minutes << "m "
                      << seconds << "s "
                      << milliseconds << "ms "
                      << nanoseconds << "ns" << std::endl;
        }
        static double getAverage(std::vector<double> times)
        {
            double sum = 0;
            for(const auto& time : times)
            {
                sum += time;
            }
            return sum / (double)times.size();
        }
        static double getSum(std::vector<double> times)
        {
            double sum = 0;
            for(const auto& time : times)
            {
                sum += time;
            }
            return sum;
        }
        static double getMax(std::vector<double> times)
        {
            double max = 0;
            for(const auto& time : times)
            {
                if(time > max)
                {
                    max = time;
                }
            }
            return max;
        }
        static double getMin(std::vector<double> times)
        {
            double min = std::numeric_limits<double>::max();
            for(const auto& time : times)
            {
                if(time < min)
                {
                    min = time;
                }
            }
            return min;
        }
        std::vector<double> m_updatePerTileTimes;
        std::vector<double> m_updateTilesTimes;
        std::vector<double> m_pickNextTileTimes;
        std::vector<double> m_placeNextTileTimes;

        std::vector<std::vector<std::shared_ptr<FieldTile>>> m_field;
        const glm::ivec2 m_fieldDimensions;

        void setupScene();
        void setupField();

        void addPlane(const glm::ivec2& pos, TileTypeEnum type);
        void updateAllTiles();
        glm::ivec2 pickNextTile();

        void start() override;
        void update() override {};
};
