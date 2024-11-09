#pragma once

#include "TileTypeData.h"

#include <glm/ext/vector_int2.hpp>
#include <glm/vec2.hpp>

namespace Engine
{
    class GeometryComponent;
    class RenderManager;
} // namespace Engine

class FieldTile
{
    public:
        FieldTile();
        ~FieldTile() = default;

        void updatePossibleTiles(const std::vector<std::vector<std::shared_ptr<FieldTile>>>& field, bool& updated);

        std::shared_ptr<Engine::GeometryComponent> setTile(
                TileTypeEnum type,
                const std::shared_ptr<Engine::RenderManager>& renderManager
        );

        void setPosition(const glm::ivec2& pos) { m_tilePos = pos; }

        const static glm::vec2 TILE_SIZE;

        const bool getIsPlaced() const { return m_tilePlaced; }

        std::vector<TileTypeEnum> getAllPossibleTiles() const { return m_possibleTiles; }

        static void setFieldSize(const glm::ivec2& fieldSize) { FIELD_SIZE = fieldSize; }

    private:
        static inline glm::ivec2 FIELD_SIZE = glm::ivec2();

        glm::ivec2 m_tilePos;
        bool m_tilePlaced;
        std::vector<TileTypeEnum> m_possibleTiles;
};
