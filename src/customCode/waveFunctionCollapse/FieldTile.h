#pragma once

#include "TileTypeData.h"

#include <glm/ext/vector_int2.hpp>
#include <glm/vec2.hpp>

namespace Engine {
    class GeometryComponent;
    class RenderManager;
}

class FieldTile
{
    public:
        FieldTile();
        ~FieldTile() = default;

        void updatePossibleTiles(const std::shared_ptr<FieldTile> (&field)[10][10], bool& updated);

        std::shared_ptr<Engine::GeometryComponent> setTile(
                TileTypeEnum type, std::shared_ptr<Engine::RenderManager> renderManager);

        void setPosition(glm::ivec2 pos) { m_tilePos = pos; }

        const static glm::vec2 TILE_SIZE;

        TileTypeEnum getCurrentTile() const { return m_currentTile; }

        std::vector<TileTypeEnum> getAllPossibleTiles() const { return m_possibleTiles; }

    private:
        glm::ivec2 m_tilePos;
        TileTypeEnum m_currentTile;
        std::vector<TileTypeEnum> m_possibleTiles;
};
