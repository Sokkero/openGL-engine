#pragma once

#include "../engine/NodeComponents/CameraComponent.h"
#include "../engine/NodeComponents/BasicNode.h"

namespace Engine { class EngineManager; class UserEventManager; }

class CameraActor : public Engine::CameraComponent
{
    public:
        CameraActor() = default;
        ~CameraActor() = default;
    private:
        void start() override;
        void update() override;

        std::shared_ptr<Engine::EngineManager> m_engineManager;
        std::shared_ptr<Engine::UserEventManager> m_userEventManager;
};
