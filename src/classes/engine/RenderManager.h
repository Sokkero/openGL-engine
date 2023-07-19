#pragma once

#include "ObjectHelper.h"

#include <string>
#include <vector>
#include <map>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    class GeometryHandler;
    class RenderManager
    {
        public:
            RenderManager();
            ~RenderManager() = default;

            std::shared_ptr<ObjectData> registerObject(const char* filePath);
            void deregisterObject(std::shared_ptr<ObjectData>& obj);
            void clearObjects();
            GLuint getUniform(ShaderType shader, const std::string& uniformName);
            std::map<ShaderType, GLuint> getShader() { return m_shaderList; };
            void renderVertices(GeometryHandler* object, const glm::mat4& mvp);
            std::map<std::string, std::shared_ptr<ObjectData>> getObjects() { return m_objectList; };

            static GLuint createVBO(std::vector<glm::vec3>& data);
            static GLuint createVBO(std::vector<glm::vec4>& data);

        private:
            void deleteObject(std::shared_ptr<ObjectData>& obj, const bool clearFromMap);

            std::map<ShaderType, GLuint> m_shaderList;
            std::map<std::string, std::shared_ptr<ObjectData>> m_objectList;
    };

}
