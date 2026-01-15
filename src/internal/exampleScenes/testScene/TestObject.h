#pragma once

#include "classes/nodeComponents/GeometryComponent.h"

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
