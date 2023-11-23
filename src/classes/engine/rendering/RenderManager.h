#pragma once

#include "../../helper/ObjectHelper.h"

#include <map>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    class GeometryComponent;
    class DiffuseLight;
    class AmbientLight;

    inline const glm::vec3 WORLD_UP = glm::vec3(0.f, 1.f, 0.f);
    inline const unsigned int VERTEX_POSITION = 0;
    inline const unsigned int VERTEX_COLOR = 1;
    inline const unsigned int VERTEX_NORMAL = 2;

    class RenderManager
    {
        public:
            RenderManager();
            ~RenderManager() = default;

            std::shared_ptr<ObjectData> registerObject(const char* filePath);
            void deregisterObject(std::shared_ptr<ObjectData>& obj);
            void clearObjects();

            GLuint registerTexture(const char* filePath);
            void deregisterTexture(GLuint tex);
            void clearTextures();

            /**
             * Shader has to consist of one .frag & one .vert shader files
             *
             * @param shaderPath full file path, without extension
             * @param shaderName The name the shader should be given
             * @return std::pair<std::string, GLuint> the loaded shaders name & ID
             */
            std::pair<std::string, GLuint> registerShader(const std::string& shaderPath, std::string shaderName);
            void deregisterShader(std::string shaderName = std::string(), GLuint shaderId = -1);
            void clearShader();

            std::map<std::string, GLuint> getShader() const { return m_shaderList; };

            void renderVertices(GeometryComponent* object, const glm::mat4& mvp);

            std::map<std::string, std::shared_ptr<ObjectData>> getObjects() { return m_objectList; };

            std::unique_ptr<AmbientLight>& getAmbientLight() { return m_ambientLight; };

            std::unique_ptr<DiffuseLight>& getDiffuseLight() { return m_diffuseLight; };

            template<typename T>
            static GLuint createVBO(std::vector<T>& data)
            {
                if(!data)
                {
                    return -1;
                }

                int dataSize = data.size() * sizeof(T);

                // Identify the vertex buffer
                GLuint vbo;
                // Generate a buffer with our identifier
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);

                // Give vertices to OpenGL
                glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);

                return vbo;
            };

        private:
            std::unique_ptr<AmbientLight> m_ambientLight;
            std::unique_ptr<DiffuseLight> m_diffuseLight;
            std::map<std::string, GLuint> m_shaderList;
            std::map<std::string, std::shared_ptr<ObjectData>> m_objectList;
            std::map<std::string, GLuint> m_textureList;

            static void deleteShader(GLuint programId)
            {
                GLint numShaders;
                glGetProgramiv(programId, GL_ATTACHED_SHADERS, &numShaders);

                // Create an array to store the shader object IDs
                auto* shaderIds = new GLuint[numShaders];

                // Get the attached shader objects
                glGetAttachedShaders(programId, numShaders, nullptr, shaderIds);

                // Detach and delete the shader objects if needed
                for(int i = 0; i < numShaders; ++i)
                {
                    GLuint shaderId = shaderIds[i];
                    glDetachShader(programId, shaderId);
                    glDeleteShader(shaderId);
                }

                // Finally, delete the program
                glDeleteProgram(programId);
                delete[](shaderIds);
            };
    };

} // namespace Engine
