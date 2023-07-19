#pragma once

namespace Engine
{
    class BasicActor
    {
        public:
            BasicActor() = default;
            virtual ~BasicActor() = default;

            virtual void start() {};
            virtual void update() {};
    };
}