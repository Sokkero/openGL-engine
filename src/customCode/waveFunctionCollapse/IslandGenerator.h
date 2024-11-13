#pragma once

#include "../../classes/nodeComponents/BasicNode.h"
#include "FieldTypeUtils.h"
#include "WafeFunctionCollapseGenerator.h"

class IslandGenerator
    : public Engine::BasicNode
    , public WafeFunctionCollapseGenerator
{
    public:
        IslandGenerator(const glm::ivec2& gridDimensions, const double& seed = 0);
        ~IslandGenerator() = default;

    protected:
        void addDefaultTiles(const bool waterOnEdges, const bool landInMiddle, const uint8_t landTilesToAdd);
        void setFieldCallback(const glm::ivec2& pos, const BasicFieldDataStruct& tileType) override;
        void start() override;

    private:
        static inline glm::vec2 FIELD_SIZE = glm::vec2(0.f);
};
