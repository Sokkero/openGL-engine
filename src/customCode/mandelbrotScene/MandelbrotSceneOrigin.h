#pragma once

#include "../../classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
}

class MandelbrotSceneOrigin : public Engine::BasicNode
{
    public:
        MandelbrotSceneOrigin() = default;
        ~MandelbrotSceneOrigin() = default;

    private:
        void start() override;
        void update() override;
};
