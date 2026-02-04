#pragma once

#include "classes/SingletonManager.h"

#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace Engine
{
    class GridShader;
    class DebugLineShader;
    class RenderManager;

    namespace Ui
    {
        class UiDebugWindow;
    }

    class DebugDrawManager : public SingletonBase
    {
            struct Line
            {
                    glm::vec3 startPoint;
                    glm::vec3 endPoint;
                    double startTime;
                    double duration; // In ms
            };

        public:
            DebugDrawManager();
            ~DebugDrawManager();

            void drawGrid();
            void drawDebugUiWindows();

            // Time in ms, 0 = 1frame, -1 = infinite, points in global space
            void drawDebugLine(const glm::vec3& startPoint, const glm::vec3& endPoint, double duration);

            void drawLines();

            void addDebugWindow(const std::shared_ptr<Ui::UiDebugWindow>& window);
            void removeDebugWindow(uint id);

        private:
            void checkTimes();
            void refreshLinePoints();

            std::vector<glm::vec3> m_linePoints;
            bool m_areLinesDirty;

            std::vector<Line> m_lines;
            GLuint m_linesVbo;

            std::unique_ptr<DebugLineShader> m_lineShader;
            std::unique_ptr<GridShader> m_gridShader;

            std::map<uint, std::shared_ptr<Ui::UiDebugWindow>> m_debugWindows;
    };
} // namespace Engine
