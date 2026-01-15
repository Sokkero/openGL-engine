#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <memory>

namespace Engine
{
    class GeometryComponent;

    struct RenderUtils
    {
            static inline const glm::vec3 WORLD_UP = glm::vec3(0.f, 1.f, 0.f);

            /* The following attrib ids are engine-reserved:
                - 0-2 for shaders using meshes
                - 3-10 for shaders of render type static or dynamic (used for instancing)
             */
            static inline const GLuint GLOBAL_ATTRIB_ID_VERTEXPOSITION = 0;
            static inline const GLuint GLOBAL_ATTRIB_ID_VERTEXUV = 1;
            static inline const GLuint GLOBAL_ATTRIB_ID_VERTEXNORMAL = 2;
            static inline const GLuint GLOBAL_ATTRIB_ID_MATRICES = 3;
            static inline const GLuint GLOBAL_ATTRIB_ID_ADDITIONAL_SHADER_DATA = 7;

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
