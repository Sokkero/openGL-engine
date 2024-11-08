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
        void updateAllTiles();
        glm::ivec2 pickNextTile();

        void start() override;
        void update() override {};
};
