#pragma once

#include <Arca/SignalTraits.h>

namespace Atmos::Render
{
    class Camera;

    class CameraMoved
    {
    public:
        Camera& camera;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CameraMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static const TypeName typeName;
    };
}