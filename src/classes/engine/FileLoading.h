#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Engine
{
    static bool loadFileOBJ(
            const char* filePath,
            std::vector<glm::vec3>& vertices,
            std::vector<glm::vec2>& uvs,
            std::vector<glm::vec3>& normals
    )
    {
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3> temp_vertices, temp_normals;
        std::vector<glm::vec2> temp_uvs;

        FILE* file = fopen(filePath, "r");
        if (file == nullptr)
        {
            printf("Impossible to open the file !\n");
            return false;
        }

        while (true)
        {
            char line[128];
            if (fgets(line, 128, file) == nullptr)
            {
                break;
            }

            //read first word
            char firstWord[128];
            sscanf(line, "%s", firstWord);

            if (strcmp(firstWord, "v") == 0)
            {
                glm::vec3 vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            }
            else if (strcmp(firstWord, "vt") == 0)
            {
                glm::vec2 uv;
                sscanf(line, "vt %f %f", &uv.x, &uv.y);
                temp_uvs.push_back(uv);
            }
            else if (strcmp(firstWord, "vn") == 0)
            {
                glm::vec3 normal;
                sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            }
            else if (strcmp(firstWord, "f") == 0)
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = sscanf(
                        line,
                        "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertexIndex[0],
                        &uvIndex[0],
                        &normalIndex[0],
                        &vertexIndex[1],
                        &uvIndex[1],
                        &normalIndex[1],
                        &vertexIndex[2],
                        &uvIndex[2],
                        &normalIndex[2]
                );
                if (matches != 9)
                {
                    matches = sscanf(
                            line,
                            "f %d//%d %d//%d %d//%d",
                            &vertexIndex[0],
                            &normalIndex[0],
                            &vertexIndex[1],
                            &normalIndex[0],
                            &vertexIndex[2],
                            &normalIndex[0]
                    );
                    if (matches != 6)
                    {
                        matches = sscanf(
                                line,
                                "f %d/%d %d/%d %d/%d",
                                &vertexIndex[0],
                                &uvIndex[0],
                                &vertexIndex[1],
                                &uvIndex[0],
                                &vertexIndex[2],
                                &uvIndex[0]
                        );
                        if (matches != 6)
                        {
                            matches = sscanf(line, "f %d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                            vertexIndices.push_back(vertexIndex[0]);
                            vertexIndices.push_back(vertexIndex[1]);
                            vertexIndices.push_back(vertexIndex[2]);
                            continue;
                        }
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        uvIndices.push_back(uvIndex[0]);
                        uvIndices.push_back(uvIndex[1]);
                        uvIndices.push_back(uvIndex[2]);
                        continue;
                    }
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                    continue;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                continue;
            }
        }

        for (unsigned int i = 0; i < vertexIndices.size(); i++)
        {
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[vertexIndex - 1];
            vertices.push_back(vertex);

            if (uvIndices.size() > i)
            {
                unsigned int uvIndex = uvIndices[i];
                glm::vec2 uv = temp_uvs[uvIndex - 1];
                uvs.push_back(uv);
            }

            if (normalIndices.size() > i)
            {
                unsigned int normalIndex = normalIndices[i];
                glm::vec3 normal = temp_normals[normalIndex - 1];
                normals.push_back(normal);
            }
        }

        return true;
    }

    //TODO: make it possible to use textures
    static const bool loadFileDDS(
            char* filePath,
            std::vector<glm::vec3>& vertices,
            std::vector<glm::vec2>& uvs,
            std::vector<glm::vec3>& normals
    );
    static const bool loadFileTGA(
            char* filePath,
            std::vector<glm::vec3>& vertices,
            std::vector<glm::vec2>& uvs,
            std::vector<glm::vec3>& normals
    );
    static const GLuint loadFileBMP(char* filePath);
}