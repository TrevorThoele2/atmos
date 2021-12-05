#pragma once

#include <Arca/Curator.h>

#include "RequestField.h"
#include "ModifyEntityBoundary.h"
#include "ModifyWorldProperties.h"
#include "RetrieveWorldProperties.h"
#include "WorldManager.h"
#include "Work.h"

namespace Atmos::World
{
    class Curator final : public Arca::Curator
    {
    public:
        Curator(Init init, Manager& manager);

        void Handle(const Work&);
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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::World::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::World::RequestField,
            Atmos::World::ModifyEntityBoundary,
            Atmos::World::ModifyProperties,
            Atmos::World::RetrieveProperties>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::World::Curator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::World::Curator>;
    };
}