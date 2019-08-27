#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "UnownedProvider.h"
#include "WorldManager.h"

namespace Atmos::World
{
    class WorldManagerProvider final
        : public Arca::ClosedTypedRelicAutomation<WorldManagerProvider>
        , public UnownedProvider<WorldManager>
    {};
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::WorldManagerProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::World::WorldManagerProvider, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::World::WorldManagerProvider, BinaryArchive>
    {};
}
