#pragma once

#include "../../classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
}

class WafeFunctionCollapseSceneOrigin : public Engine::BasicNode
{
    public:
        WafeFunctionCollapseSceneOrigin() = default;
        ~WafeFunctionCollapseSceneOrigin() = default;

    private:
        void start() override;
        void update() override;
};
