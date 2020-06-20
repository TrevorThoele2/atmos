#pragma once

#include <Arca/Signal.h>

namespace Atmos::Render
{
    class Camera;

    class CameraMoved
    {
    public:
        Arca::Index<Camera> camera;

        explicit CameraMoved(Arca::Index<Camera> camera) : camera(std::move(camera))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CameraMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "Atmos::Render::CameraMoved";
    };
}