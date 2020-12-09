#pragma once

#include <Arca/Curator.h>

#include "Camera.h"

#include "ReconstructGraphics.h"
#include "Work.h"

#include "DebugValue.h"

namespace Atmos::Render
{
    class CameraCurator final : public Arca::Curator
    {
    public:
        explicit CameraCurator(Init init);

        void Handle(const Work& command);
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
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::CameraCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::CameraCurator>;
    };
}