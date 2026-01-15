#pragma once

#include "classes/nodeComponents/BasicNode.h"

namespace Engine
{
    class EngineManager;
    class RenderComponent;
} // namespace Engine

class TestObject;

class TestSceneOrigin : public Engine::BasicNode
{
    public:
        TestSceneOrigin() = default;
        ~TestSceneOrigin() = default;

    private:
        std::shared_ptr<Engine::EngineManager> m_engineManager;

        std::shared_ptr<Engine::RenderComponent> m_tree;
        std::shared_ptr<TestObject> m_ape;

        void start() override;
        void update() override;
};
