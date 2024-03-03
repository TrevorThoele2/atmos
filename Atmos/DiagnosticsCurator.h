#pragma once

#include <Arca/Curator.h>

#include "Work.h"

namespace Atmos::Diagnostics
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);

        void Handle(const Work& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Diagnostics::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Diagnostics::Curator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Diagnostics::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Diagnostics::Curator>;
    };
}