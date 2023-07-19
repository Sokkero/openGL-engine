#pragma once

#include "../engine/ActorHandler/BasicNode.h"
#include "../engine/ActorHandler/TransformHandler.h"
#include "../engine/ActorHandler/GeometryHandler.h"

using namespace Engine;

class TestObject : public BasicNode, public TransformHandler, public GeometryHandler
{
    public:
        TestObject() = default;
        ~TestObject() = default;
    private:
        void start() override;
        void update() override;
};
