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
        solidColor, solidTexture
    };

    struct ObjectData
    {
        ObjectData(
                int matrixID,
                GLuint vertexBuffer,
                std::vector<glm::vec3> vertexData,
                std::vector<glm::vec3> vertexNormals
        );

        int m_matrixID;
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

            std::shared_ptr<ObjectData> registerObject(ShaderType shader, const std::string& filePath);
            void deregisterObject(int objectId);
            GLuint getUniform(ShaderType shader, const std::string& uniformName);
            std::vector<std::pair<ShaderType, GLuint>> getShader() { return m_shaderList; };
            std::vector<std::pair<std::string, std::shared_ptr<ObjectData>>> getObjects() { return m_objectList; };
            void renderVertices(ObjectData* object, glm::mat4 mvp);

            static GLuint createVBO(std::vector<glm::vec3>& data);
            static GLuint createVBO(std::vector<glm::vec4>& data);

        private:
            void loadShader();

            std::vector<std::pair<ShaderType, GLuint>> m_shaderList;
            std::vector<std::pair<std::string, std::shared_ptr<ObjectData>>> m_objectList;
    };

}
