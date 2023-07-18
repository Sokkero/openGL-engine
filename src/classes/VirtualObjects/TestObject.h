#pragma once

#include "../engine/ActorHandler/BasicActor.h"
#include "../engine/ActorHandler/TransformHandler.h"
#include "../engine/ActorHandler/GeometryHandler.h"

using namespace Engine;

class TestObject : public BasicActor, public TransformHandler, public GeometryHandler
{
    public:
        TestObject() = default;
        ~TestObject() = default;
    private:
};
