#include "DebugDrawManager.h"

#include "classes/nodeComponents/UiDebugWindow.h"
#include "resources/shader/debugLineShader/DebugLineShader.h"
#include "resources/shader/gridShader/GridShader.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    DebugDrawManager::DebugDrawManager()
        : m_gridShader(nullptr)
        , m_lineShader(nullptr)
        , m_areLinesDirty(false)
    {
        glGenBuffers(1, &m_linesVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_linesVbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        RenderUtils::checkForGLError();
    }

    DebugDrawManager::~DebugDrawManager()
    {
        glDeleteBuffers(1, &m_linesVbo);
    }

    void DebugDrawManager::drawGrid()
    {
        if(!m_gridShader)
        {
            m_gridShader = std::make_unique<GridShader>();
        }

        m_gridShader->renderObject(nullptr, nullptr);
    }

    void DebugDrawManager::drawDebugUiWindows()
    {
        for(auto& window : m_debugWindows)
        {
            window.second->drawUi();
        }
    }

    void DebugDrawManager::addDebugWindow(const std::shared_ptr<Ui::UiDebugWindow>& window)
    {
        m_debugWindows[window->getNodeId()] = window;
    }

    void DebugDrawManager::removeDebugWindow(uint id)
    {
        m_debugWindows.erase(id);
    }

    void DebugDrawManager::drawLines()
    {
        if(m_areLinesDirty)
        {
            refreshLinePoints();
        }

        if(m_linePoints.empty())
        {
            return;
        }

        if(!m_lineShader)
        {
            m_lineShader = std::make_unique<DebugLineShader>();
        }

        m_lineShader->renderLines(m_linesVbo, m_linePoints.size());

        checkTimes();
    }

    void DebugDrawManager::checkTimes()
    {
        bool wasInvalidated = false;
        m_lines
                .erase(std::remove_if(
                               m_lines.begin(),
                               m_lines.end(),
                               [&wasInvalidated](auto& obj) -> bool
                               {
                                   if(obj.duration < 0)
                                   {
                                       return false;
                                   }

                                   int64_t lifeTime = TimeUtils::GetDurationSince(obj.startTime);
                                   if(obj.duration <= lifeTime || obj.duration == 0)
                                   {
                                       wasInvalidated = true;
                                       return true;
                                   }
                                   return false;
                               }
                       ),
                       m_lines.end());

        m_areLinesDirty = m_areLinesDirty || wasInvalidated;
    }

    void DebugDrawManager::refreshLinePoints()
    {
        m_linePoints.clear();
        for(const auto& line : m_lines)
        {
            m_linePoints.push_back(line.startPoint);
            m_linePoints.push_back(line.endPoint);
        }

        bool isEmpty = m_linePoints.empty();

        glBindBuffer(GL_ARRAY_BUFFER, m_linesVbo);
        glBufferData(
                GL_ARRAY_BUFFER,
                m_linePoints.size() * sizeof(glm::vec3),
                isEmpty ? nullptr : &m_linePoints[0],
                GL_DYNAMIC_DRAW
        );

        m_areLinesDirty = false;
    }

    void DebugDrawManager::drawDebugLine(const glm::vec3& startPoint, const glm::vec3& endPoint, int64_t duration)
    {
        Line line;
        line.startPoint = startPoint;
        line.endPoint = endPoint;
        line.duration = duration;
        line.startTime = TimeUtils::GetSystemsTimestamp();

        m_lines.push_back(line);

        m_areLinesDirty = true;
    }
} // namespace Engine
