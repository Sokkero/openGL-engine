#pragma once

#include "../engine/NodeComponents/CameraComponent.h"
#include "../engine/NodeComponents/BasicNode.h"

namespace Engine { class EngineManager; class UserEventManager; }

class CameraActor : public Engine::BasicNode, public Engine::CameraComponent
{
    public:
        CameraActor() = default;
        ~CameraActor() = default;
    private:
        void start() override;
        void update() override;

        Engine::EngineManager* m_engineManager;
        Engine::UserEventManager* m_userEventManager;
};
