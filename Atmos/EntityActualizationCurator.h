#pragma once

#include <Arca/Curator.h>

#include "EntityPrototype.h"
#include "Work.h"

namespace Atmos::Entity
{
    class ActualizationCurator final : public Arca::Curator
    {
    public:
        explicit ActualizationCurator(Init init);

        void Handle(const Work& command);
    private:
        Arca::Batch<Prototype> prototypes;

        bool worked = false;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::ActualizationCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Entity::ActualizationCurator"; }
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Entity::ActualizationCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::ActualizationCurator>;
    };
}