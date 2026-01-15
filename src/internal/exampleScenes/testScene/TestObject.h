#pragma once

#include "classes/nodeComponents/RenderComponent.h"

using namespace Engine;

class TestObject : public RenderComponent
{
    public:
        TestObject() = default;
        ~TestObject() = default;

    private:
        void start() override;
        void update() override;
};
