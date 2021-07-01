#pragma once

#include <Arca/Curator.h>

#include "Work.h"

namespace Atmos::Render
{
    class EndCurator final : public Arca::Curator
    {
    public:
        explicit EndCurator(Init init);

        void Handle(const Work& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::EndCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::EndCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::EndCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::EndCurator>;
    };
}