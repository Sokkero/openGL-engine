#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

namespace Engine
{
    /**
     * Loads an OBJ file and extracts the vertex positions, texture coordinates, and normals.
     *
     * @param filePath The path to the OBJ file.
     * @param vertices The vector to store the vertex positions.
     * @param uvs The vector to store the texture coordinates.
     * @param normals The vector to store the normals.
     * @return True if the file was loaded successfully, false otherwise.
     */
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
        if(file == nullptr)
        {
            std::cout << "Couldn't open file [" << filePath << "]" << std::endl;
            return false;
        }

        while(true)
        {
            char line[128];
            if(fgets(line, 128, file) == nullptr)
            {
                break;
            }

            // read first word
            char firstWord[128];
            sscanf(line, "%s", firstWord);

            if(strcmp(firstWord, "v") == 0)
            {
                glm::vec3 vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            }
            else if(strcmp(firstWord, "vt") == 0)
            {
                glm::vec2 uv;
                sscanf(line, "vt %f %f", &uv.x, &uv.y);
                temp_uvs.push_back(uv);
            }
            else if(strcmp(firstWord, "vn") == 0)
            {
                glm::vec3 normal;
                sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            }
            else if(strcmp(firstWord, "f") == 0)
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches =
                        sscanf(line,
                               "f %d/%d/%d %d/%d/%d %d/%d/%d",
                               &vertexIndex[0],
                               &uvIndex[0],
                               &normalIndex[0],
                               &vertexIndex[1],
                               &uvIndex[1],
                               &normalIndex[1],
                               &vertexIndex[2],
                               &uvIndex[2],
                               &normalIndex[2]);
                if(matches != 9)
                {
                    matches =
                            sscanf(line,
                                   "f %i//%i %i//%i %i//%i",
                                   &vertexIndex[0],
                                   &normalIndex[0],
                                   &vertexIndex[1],
                                   &normalIndex[1],
                                   &vertexIndex[2],
                                   &normalIndex[2]);
                    if(matches != 6)
                    {
                        matches =
                                sscanf(line,
                                       "f %i/%i %i/%i %i/%i",
                                       &vertexIndex[0],
                                       &uvIndex[0],
                                       &vertexIndex[1],
                                       &uvIndex[1],
                                       &vertexIndex[2],
                                       &uvIndex[2]);
                        if(matches != 6)
                        {
                            sscanf(line, "f %i %i %i", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
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

        for(unsigned int i = 0; i < vertexIndices.size(); i++)
        {
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[vertexIndex - 1];
            vertices.push_back(vertex);

            if(uvIndices.size() > i)
            {
                unsigned int uvIndex = uvIndices[i];
                glm::vec2 uv = temp_uvs[uvIndex - 1];
                uvs.push_back(uv);
            }

            if(normalIndices.size() > i)
            {
                unsigned int normalIndex = normalIndices[i];
                glm::vec3 normal = temp_normals[normalIndex - 1];
                normals.push_back(normal);
            }
        }

        return true;
    }

    /**
     * Loads a DDS file and returns the OpenGL texture ID.
     *
     * @param filePath The path to the DDS file.
     * @return The OpenGL texture ID if the file was loaded successfully, -1 otherwise.
     */
    static GLuint loadFileDDS(const char* filePath)
    {
        unsigned char header[124];

        FILE* fp;

        /* try to open the file */
        fp = fopen(filePath, "rb");
        if(fp == nullptr)
        {
            printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n",
                   filePath);
            getchar();
            return -1;
        }

        /* verify the type of file */
        char filecode[4];
        fread(filecode, 1, 4, fp);
        if(strncmp(filecode, "DDS ", 4) != 0)
        {
            fclose(fp);
            return -1;
        }

        /* get the surface desc */
        fread(&header, 124, 1, fp);

        unsigned int height = *(unsigned int*)&(header[8]);
        unsigned int width = *(unsigned int*)&(header[12]);
        unsigned int linearSize = *(unsigned int*)&(header[16]);
        unsigned int mipMapCount = *(unsigned int*)&(header[24]);
        unsigned int fourCC = *(unsigned int*)&(header[80]);

        unsigned char* buffer;
        unsigned int bufsize;
        /* how big is it going to be including all mipmaps? */
        bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
        buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
        fread(buffer, 1, bufsize, fp);
        /* close the file pointer */
        fclose(fp);

        unsigned int format;
        switch(fourCC)
        {
            case FOURCC_DXT1:
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                break;
            case FOURCC_DXT3:
                format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                break;
            case FOURCC_DXT5:
                format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                break;
            default:
                free(buffer);
                return 0;
        }

        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        unsigned int offset = 0;

        /* load the mipmaps */
        for(unsigned int level = 0; level < mipMapCount && (width || height); ++level)
        {
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
            glCompressedTexImage2D(
                    GL_TEXTURE_2D,
                    GLint(level),
                    format,
                    GLsizei(width),
                    GLsizei(height),
                    0,
                    GLsizei(size),
                    buffer + offset
            );

            offset += size;
            width /= 2;
            height /= 2;

            // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
            if(width < 1)
            {
                width = 1;
            }
            if(height < 1)
            {
                height = 1;
            }
        }

        free(buffer);

        return textureID;
    }

    /**
     * Loads a BMP file and returns the OpenGL texture ID.
     *
     * @param filePath The path to the BMP file.
     * @return The OpenGL texture ID if the file was loaded successfully, -1 otherwise.
     */
    static GLuint loadFileBMP(const char* filePath)
    {
        // Data read from the header of the BMP file
        unsigned int width, height;
        unsigned char* data;
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;

        // Open the file
        FILE* file = fopen(filePath, "rb");
        if(!file)
        {
            std::cout << "Couldn't open file [" << filePath << "]" << std::endl;
            return -1;
        }

        // Read the header, i.e. the 54 first bytes

        // If less than 54 bytes are read, problem
        if(fread(header, 1, 54, file) != 54)
        {
            std::cout << "BMP file is not correct [" << filePath << "]" << std::endl;
            fclose(file);
            return -1;
        }
        // A BMP files always begins with "BM"
        if(header[0] != 'B' || header[1] != 'M')
        {
            std::cout << "BMP file is not correct [" << filePath << "]" << std::endl;
            fclose(file);
            return -1;
        }
        // Make sure this is a 24bpp file
        if(*(int*)&(header[0x1E]) != 0)
        {
            std::cout << "BMP file is not correct [" << filePath << "]" << std::endl;
            fclose(file);
            return -1;
        }
        if(*(int*)&(header[0x1C]) != 24)
        {
            std::cout << "BMP file is not correct [" << filePath << "]" << std::endl;
            fclose(file);
            return -1;
        }

        // Read the information about the image
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]) * 3;
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if(imageSize == 0)
        {
            imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
        }
        if(dataPos == 0)
        {
            dataPos = 54; // The BMP header is done that way
        }

        // Create a buffer
        data = new unsigned char[imageSize];

        // Read the actual data from the file into the buffer
        fseek(file, dataPos, SEEK_SET);
        fread(data, 1, imageSize, file);

        // Everything is in memory now, the file can be closed.
        fclose(file);

        // TODO: read up what all of this does in more detail
        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLsizei(width), GLsizei(height), 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        // OpenGL has now copied the data. Free our own version
        delete[] data;

        // Poor filtering...
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Nice trilinear filtering ...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // ... which requires mipmaps. Generate them automatically.
        glGenerateMipmap(GL_TEXTURE_2D);

        // Return the ID of the texture we just created
        return textureID;
    }
} // namespace Engine