#pragma once

#include <Arca/Curator.h>

#include "RequestField.h"
#include "ModifyEntityBoundary.h"
#include "ModifyWorldProperties.h"
#include "RetrieveWorldProperties.h"
#include "WorldManager.h"

namespace Atmos::World
{
    class Curator final : public Arca::Curator
    {
    public:
        Curator(Init init, Manager& manager);

        void Handle(const RequestField& command);
        void Handle(const ModifyEntityBoundary& command);
        void Handle(const ModifyProperties& command);
        std::vector<Property> Handle(const RetrieveProperties& command);
    private:
        Manager* manager;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::World::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::World::Curator"; }
        using HandledCommands = HandledCommands<
            Atmos::World::RequestField,
            Atmos::World::ModifyEntityBoundary,
            Atmos::World::ModifyProperties,
            Atmos::World::RetrieveProperties>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::World::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::World::Curator>;
    };
}