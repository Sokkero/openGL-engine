#pragma once

#include "../engine/ActorHandler/BasicActor.h"

class SceneOrigin : public Engine::BasicActor
{
    public:
        SceneOrigin() = default;
        ~SceneOrigin() = default;
    private:
        void start() override;

};

