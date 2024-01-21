#pragma once

#include <tuple>

typedef std::tuple<unsigned short, unsigned short, unsigned short> triData;

static bool depthSortTrianglesAlgorithm(
        const glm::vec3& cameraPos,
        const glm::vec3& nodePos,
        const std::vector<glm::vec3>& vertices,
        const triData& triA,
        const triData& triB
)
{
    auto aPos = (vertices[std::get<0>(triA)] + vertices[std::get<1>(triA)] + vertices[std::get<2>(triA)]);
    aPos.x = aPos.x / 3;
    aPos.y = aPos.y / 3;
    aPos.z = aPos.z / 3;
    aPos += nodePos;

    auto bPos = (vertices[std::get<0>(triB)] + vertices[std::get<1>(triB)] + vertices[std::get<2>(triB)]);
    bPos.x = bPos.x / 3;
    bPos.y = bPos.y / 3;
    bPos.z = bPos.z / 3;
    bPos += nodePos;

    const auto& distanceA = glm::distance(aPos, cameraPos);
    const auto& distanceB = glm::distance(bPos, cameraPos);

    return distanceA < distanceB;
}