#pragma once

#include <Arca/Curator.h>

#include "MoveBounds.h"
#include "ScaleBounds.h"
#include "RotateBounds.h"

namespace Atmos
{
    class BoundsCurator final : public Arca::Curator
    {
    public:
        BoundsCurator(Init init);
    public:
        void Handle(const MoveBounds& command);
        void Handle(const ScaleBounds& command);
        void Handle(const RotateBounds& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::BoundsCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::BoundsCurator";
        using HandledCommands = HandledCommands<
            Atmos::MoveBounds,
            Atmos::ScaleBounds,
            Atmos::RotateBounds>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::BoundsCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::BoundsCurator, BinaryArchive>
    {};
}