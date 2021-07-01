#pragma once

#include <Arca/Curator.h>

#include "Work.h"

namespace Atmos::Render
{
    class StartCurator final : public Arca::Curator
    {
    public:
        explicit StartCurator(Init init);

        void Handle(const Work& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::StartCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::StartCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::StartCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::StartCurator>;
    };
}