#pragma once

#include "../../helper/ObjectData.h"
#include "lighting/AmbientLightUbo.h"
#include "lighting/DiffuseLightUbo.h"

#include <map>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    class GeometryComponent;
    class Shader;

    inline const glm::vec3 WORLD_UP = glm::vec3(0.f, 1.f, 0.f);

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

            std::map<std::string, GLuint> getShader() const { return m_shaderList; }

            std::map<std::string, std::shared_ptr<ObjectData>> getObjects() { return m_objectList; };

            std::shared_ptr<Lighting::AmbientLightUbo>& getAmbientLightUbo() { return m_ambientLightUbo; };

            std::shared_ptr<Lighting::DiffuseLightUbo>& getDiffuseLightUbo() { return m_diffuseLightUbo; };

            bool getWireframeMode() const { return m_showWireframe; };

            void setWireframeMode(bool toggle);

            template<typename T>
            static GLuint createVBO(std::vector<T>& data)
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
            };

        private:
            std::shared_ptr<Lighting::AmbientLightUbo> m_ambientLightUbo;
            std::shared_ptr<Lighting::DiffuseLightUbo> m_diffuseLightUbo;
            std::map<std::string, GLuint> m_shaderList;
            std::map<std::string, std::shared_ptr<ObjectData>> m_objectList;
            std::map<std::string, GLuint> m_textureList;
            bool m_showWireframe;
    };

} // namespace Engine
