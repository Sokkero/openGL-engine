#pragma once

#include "../../classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
}

class MandelbrotUbo;

class MandelbrotSceneOrigin : public Engine::BasicNode
{
    public:
        MandelbrotSceneOrigin();
        ~MandelbrotSceneOrigin() = default;

        void increaseZoom();
        void decreaseZoom();
        void moveCam(glm::vec2 movement);

    private:
        void start() override;
        void update() override;

        std::shared_ptr<MandelbrotUbo> m_mandelbrotUbo;
};
