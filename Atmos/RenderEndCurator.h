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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::EndCurator";
        using HandledCommands = HandledCommands<
            Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::EndCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::EndCurator>;
    };
}