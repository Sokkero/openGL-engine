#pragma once

#include "../engine/NodeComponents/BasicNode.h"
#include "../engine/NodeComponents/GeometryComponent.h"
#include "../engine/NodeComponents/TransformComponent.h"

using namespace Engine;

class TestObject : public GeometryComponent
{
    public:
        TestObject() = default;
        ~TestObject() = default;

    private:
        void start() override;
        void update() override;
};
