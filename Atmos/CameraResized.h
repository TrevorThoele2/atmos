#pragma once

#include <Arca/Signal.h>
#include <Arca/Index.h>

namespace Atmos::Render
{
    class Camera;

    class CameraResized
    {
    public:
        Arca::Index<Camera> camera;

        explicit CameraResized(Arca::Index<Camera> camera) : camera(std::move(camera))
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CameraResized>
    {
        static const ObjectType objectType = ObjectType::Signal;
        static inline const TypeName typeName = "CameraResized";
    };
}