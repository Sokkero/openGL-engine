#pragma once

#include "../../classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
}

class SceneOrigin : public Engine::BasicNode
{
    public:
        SceneOrigin() = default;
        ~SceneOrigin() = default;

    private:
        void start() override;
        void update() override;
};
