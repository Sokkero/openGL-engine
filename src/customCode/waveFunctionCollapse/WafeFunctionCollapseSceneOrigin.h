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
        WafeFunctionCollapseSceneOrigin();
        ~WafeFunctionCollapseSceneOrigin() = default;

    private:
        std::shared_ptr<FieldTile> m_field[10][10];

        void setupScene();
        void setupField();

        void addPlane(glm::ivec2 pos, TileTypeEnum type);
        void updateAllTiles();
        glm::ivec2 pickNextTile();

        void start() override;
        void update() override {};
};
