#pragma once

#include <Arca/Curator.h>

#include "Camera.h"

#include "ReconstructGraphics.h"

#include "DebugValue.h"

namespace Atmos::Render
{
    class CameraCurator final : public Arca::Curator
    {
    public:
        explicit CameraCurator(Init init);

        void Work();
    private:
        Arca::Index<Camera> camera;
    private:
        Debug::Value debugCenterX;
        Debug::Value debugCenterY;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::CameraCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Render::CameraCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::CameraCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Render::CameraCurator, BinaryArchive>
    {};
}