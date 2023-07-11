
#include "RenderManager.h"
#include "loadShader.h"

namespace Engine {

    ObjectData::ObjectData(
            int matrixID,
            GLuint vertexBuffer,
            std::vector<glm::vec3> vertexData,
            std::vector<glm::vec3> vertexNormals
    )
    : m_matrixID(matrixID)
    , m_vertexBuffer(vertexBuffer)
    , m_vertexData(std::move(vertexData))
    , m_vertexNormals(std::move(vertexNormals))
    {
    }

    void RenderManager::loadShader() {
        GLuint tempShaderID;

        tempShaderID = LoadShaders( "resources/shader/simpleVertexShader.vert", "resources/shader/simpleFragmentShader.frag" );
        m_shaderList.emplace_back(ShaderType::solidColor, tempShaderID);

        tempShaderID = LoadShaders( "resources/shader/simpleVertexShader.vert", "resources/shader/simpleFragmentShader.frag" );
        m_shaderList.emplace_back(ShaderType::solidTexture, tempShaderID);
    }

    GLuint RenderManager::getUniform(ShaderType type, const std::string& uniformName)
    {
        for ( auto& shader : m_shaderList )
        {
            if ( shader.first == type)
                return glGetUniformLocation(shader.second, uniformName.c_str());
        }
        return 0;
    }

    std::shared_ptr<ObjectData> RenderManager::registerObject(ShaderType shader, const std::string& filePath)
    {
        for ( auto& object : m_objectList )
        {
            if ( object.first == filePath )
            {
                return object.second;
            }
        }
        //TODO: rewrite this
        /*

        std::vector<glm::vec3> vertexData, vertexNormals;
        std::vector<glm::vec2> textureData;

        //fileLoader::loadFileOBJ(filePath, vertexData, textureData, vertexNormals);

        GLuint vertexBuffer = createVBO(vertexData);
        const int matrixId = int(getUniform(shader, "MVP"));
        const int objId = int(m_shaderList.size());

        std::shared_ptr<ObjectData> newObject = std::make_shared<ObjectData>(objId, shader, matrixId, vertexBuffer, 0, vertexData, vertexNormals);

        m_objectList.emplace_back(filePath, newObject);

        return newObject; */
    }

    void RenderManager::deregisterObject(int objectId)
    {
        //TODO: rewrite this
        /*
        if (objectId < m_objectList.size()) {
            return;
        }

        auto& obj = m_objectList[objectId].second;
        GLuint buffer[2] = {obj->m_vertexBuffer, obj->m_colorBuffer};

        glDeleteBuffers(2, buffer);

        m_objectList.erase(m_objectList.begin() + objectId); */
    }

    void RenderManager::renderVertices(ObjectData* object, glm::mat4 mvp)
    {
        //TODO: rewrite this
        /*
        for ( auto& shader : m_shaderList)
        {
            if ( shader.first == object->m_shader)
                glUseProgram(shader.second);
        }

        // Send our transformation to the currently bound rendering, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(object->m_matrixID, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, object->m_vertexBuffer);
        glVertexAttribPointer(
                0,             // No particular reason for 0, but must match the layout in the rendering
                3,             // Size
                GL_FLOAT,      // Type
                GL_FALSE,      // Normalized?
                0,             // Stride
                (void*)nullptr // Array buffer offset
        );

        switch (object->m_shader) {
            case ShaderType::solidColor:
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, object->m_colorBuffer);
                glVertexAttribPointer(
                        1,             // No particular reason for 1, but must match the layout in the rendering
                        4,             // Size
                        GL_FLOAT,      // Type
                        GL_FALSE,      // Normalized?
                        0,             // Stride
                        (void*) nullptr // Array buffer offset
                );
                break;
            case ShaderType::solidTexture:
                break;
        }

        // Drawing the object
        glDrawArrays(GL_TRIANGLES, 0, object->getVertexCount() / 3); // Start at vertex 0 -> 3 Vertices -> 1 Triangle
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1); */
    }

    GLuint RenderManager::createVBO(std::vector<glm::vec3> &data)
    {
        int dataSize = data.size() * sizeof(glm::vec3);

        // Identify the vertex buffer
        GLuint vbo;
        // Generate a buffer with our identifier
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Give vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);

        return vbo;
    }

    GLuint RenderManager::createVBO(std::vector<glm::vec4> &data)
    {
        int dataSize = data.size() * sizeof(glm::vec4);

        // Identify the vertex buffer
        GLuint vbo;
        // Generate a buffer with our identifier
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Give vertices to OpenGL
        glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);

        return vbo;
    }
}