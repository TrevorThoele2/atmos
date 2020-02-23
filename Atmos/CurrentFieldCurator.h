#pragma once

#include <Arca/Curator.h>

#include "DebugValue.h"

namespace Atmos::World
{
    class CurrentFieldCurator final : public Arca::Curator
    {
    public:
        explicit CurrentFieldCurator(Init init);

        void Work();
    private:
        Debug::Value debugFieldID;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::CurrentFieldCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "CurrentFieldCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::World::CurrentFieldCurator, BinaryArchive> final :
        public ArcaNullScribe<Atmos::World::CurrentFieldCurator, BinaryArchive>
    {};
}