#pragma once

#include <GL/glew.h>
#include <map>
#include <string>

namespace Engine
{
    static std::map<std::string, int> ShaderPoints = std::map<std::string, int>();

    static void AddShaderPoint(std::string name, GLuint point) { ShaderPoints.emplace(name, point); }

    static int GetShaderPoint(std::string name) { return ShaderPoints[name]; }

    static std::string GetShaderPointName(int id)
    {
        for(const auto& shaderPoint : ShaderPoints)
        {
            if(shaderPoint.second == id)
            {
                return shaderPoint.first;
            }
        }
        return "";
    }

    static void RemoveShaderPoint(std::string name) { ShaderPoints.erase(name); }
} // namespace Engine