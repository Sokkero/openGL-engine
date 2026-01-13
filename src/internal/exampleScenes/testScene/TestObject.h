#pragma once

#include "../../classes/nodeComponents/BasicNode.h"
#include "../../classes/nodeComponents/GeometryComponent.h"
#include "../../classes/nodeComponents/TransformComponent.h"

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
