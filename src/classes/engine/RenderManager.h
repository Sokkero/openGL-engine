#pragma once

#include <cstdlib>
#include <string>
#include <cstdlib>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

    enum ShaderType
    {
        solidColor, solidTexture, undefined
    };

    struct ObjectData
    {
        ObjectData(
                GLuint vertexBuffer,
                std::vector<glm::vec3> vertexData,
                std::vector<glm::vec3> vertexNormals
        );

        GLuint m_vertexBuffer;
        std::vector<glm::vec3> m_vertexData;
        std::vector<glm::vec3> m_vertexNormals;

        int getVertexCount() const { return int(m_vertexData.size()); };
    };

    class RenderManager
    {
        public:
            RenderManager() = default;
            ~RenderManager() = default;

            std::shared_ptr<ObjectData> registerObject(const std::string& filePath);
            void deregisterObject(const std::shared_ptr<ObjectData>& obj);
            GLuint getUniform(ShaderType shader, const std::string& uniformName);
            std::vector<std::pair<ShaderType, GLuint>> getShader() { return m_shaderList; };
            std::vector<std::pair<std::string, std::shared_ptr<ObjectData>>> getObjects() { return m_objectList; };
            void renderVertices(int obj); //TODO: ??

            static GLuint createVBO(std::vector<glm::vec3>& data);
            static GLuint createVBO(std::vector<glm::vec4>& data);

        private:
            void loadShader();

            std::vector<std::pair<ShaderType, GLuint>> m_shaderList;
            std::vector<std::pair<std::string, std::shared_ptr<ObjectData>>> m_objectList;
    };

}
