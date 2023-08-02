#pragma once

#include "../engine/NodeComponents/BasicNode.h"
#include "../engine/NodeComponents/CameraComponent.h"

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
        void start() override;
        void update() override;
};
