#pragma once

#include "../../classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
}

class MandelbrotScene : public Engine::BasicNode
{
    public:
        MandelbrotScene() = default;
        ~MandelbrotScene() = default;

    private:
        void start() override;
        void update() override;
};
