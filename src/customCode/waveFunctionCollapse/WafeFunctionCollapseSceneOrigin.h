#pragma once

#include "../../classes/nodeComponents/BasicNode.h"
#include "FieldTile.h"

namespace Engine
{
    class EngineManager;
}

class WafeFunctionCollapseSceneOrigin : public Engine::BasicNode
{
    public:
        WafeFunctionCollapseSceneOrigin(const glm::ivec2& fieldDimension);
        ~WafeFunctionCollapseSceneOrigin() = default;

    private:
        std::vector<std::vector<std::shared_ptr<FieldTile>>> m_field;
        const glm::ivec2 m_fieldDimensions;

        void setupScene();
        void setupField();

        void addPlane(const glm::ivec2& pos, TileTypeEnum type);
        glm::ivec2 pickNextTile();
        glm::ivec2 getTileForTileType(TileTypeEnum tile);

        void addDefaultTiles(const bool waterOnEdges = true, const bool landInMiddle = true, const uint8_t landTilesToAdd = 2);

        void start() override;
        void update() override {};
};
