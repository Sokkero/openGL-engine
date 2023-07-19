#pragma once

#include "../engine/NodeComponents/BasicNode.h"
#include "../engine/NodeComponents/TransformComponent.h"
#include "../engine/NodeComponents/GeometryComponent.h"

using namespace Engine;

class TestObject : public BasicNode, public TransformComponent, public GeometryComponent
{
    public:
        TestObject() = default;
        ~TestObject() = default;
    private:
        void start() override;
        void update() override;
};
