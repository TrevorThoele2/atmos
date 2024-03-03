#pragma once

#include <Arca/Signal.h>
#include <utility>

namespace Atmos::Render
{
    class Camera;

    class CameraMoved
    {
    public:
        Arca::GlobalIndex<Camera> camera;

        explicit CameraMoved(Arca::GlobalIndex<Camera> camera) : camera(std::move(camera))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CameraMoved>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "CameraMoved";
    };
}