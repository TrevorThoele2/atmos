#pragma once

#include <Arca/Curator.h>

#include "ModifyProperties.h"

namespace Atmos
{
    class DataCurator final : public Arca::Curator
    {
    public:
        explicit DataCurator(Init init);

        void Handle(const ModifyProperties& command);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::DataCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::DataCurator";
        using HandledCommands = HandledCommands<
            Atmos::ModifyProperties>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::DataCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::DataCurator>;
    };
}