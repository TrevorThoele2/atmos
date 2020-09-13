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
        static inline const TypeName typeName = "Atmos::World::CurrentFieldCurator";
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::World::CurrentFieldCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::World::CurrentFieldCurator>;
    };
}