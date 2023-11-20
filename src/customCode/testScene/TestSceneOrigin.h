#pragma once

#include "../../classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
}

class TestSceneOrigin : public Engine::BasicNode
{
    public:
        TestSceneOrigin() = default;
        ~TestSceneOrigin() = default;

    private:
        void start() override;
        void update() override;
};
