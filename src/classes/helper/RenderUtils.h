#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <memory>

namespace Engine
{
    class GeometryComponent;

    struct RenderUtils
    {
            static const glm::vec3 WORLD_UP;

            static void checkForGLError();

            template<typename T>
            static GLuint createVertexBufferObject(std::vector<T>& data)
            {
                int dataSize = data.size() * sizeof(T);

                // Identify the vertex buffer
                GLuint vbo;
                // Generate a buffer with our identifier
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);

                // Give vertices to OpenGL
                glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);

                return vbo;
            }

            static bool nodeDistanceSortingAlgorithm(
                    const std::shared_ptr<GeometryComponent>& a,
                    const std::shared_ptr<GeometryComponent>& b,
                    const glm::vec3& cameraPosition
            );
    };
} // namespace Engine
