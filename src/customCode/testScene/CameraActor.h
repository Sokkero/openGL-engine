#pragma once

#include "../../classes/nodeComponents/BasicNode.h"
#include "../../classes/nodeComponents/CameraComponent.h"

namespace Engine
{
    class EngineManager;
    class UserEventManager;
} // namespace Engine

class CameraActor : public Engine::CameraComponent
{
    public:
        CameraActor() = default;
        ~CameraActor() = default;

    private:
        std::shared_ptr<Engine::EngineManager> m_engineManager;

        void start() override;
        void update() override;
};
