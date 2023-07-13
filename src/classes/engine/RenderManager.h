#pragma once

#include "VirtualObject.h"
#include "ObjectHelper.h"

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

    class RenderManager
    {
        public:
            RenderManager();
            ~RenderManager() = default;

            std::shared_ptr<ObjectData> registerObject(const char* filePath);
            void deregisterObject(const std::shared_ptr<ObjectData>& obj);
            //TODO: Add a "clear all objects" function
            GLuint getUniform(ShaderType shader, const std::string& uniformName);
            std::vector<std::pair<ShaderType, GLuint>> getShader() { return m_shaderList; };
            void renderVertices(VirtualObject* object, const glm::mat4& mvp);
            std::vector<std::pair<std::string, std::shared_ptr<ObjectData>>> getObjects() { return m_objectList; };

            static GLuint createVBO(std::vector<glm::vec3>& data);
            static GLuint createVBO(std::vector<glm::vec4>& data);

        private:

            //TODO: turn these into a map
            std::vector<std::pair<ShaderType, GLuint>> m_shaderList;
            std::vector<std::pair<std::string, std::shared_ptr<ObjectData>>> m_objectList;
    };

}
