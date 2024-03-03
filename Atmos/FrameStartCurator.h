#pragma once

#include <Arca/Curator.h>

#include "Work.h"

namespace Atmos::Frame
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
    struct Traits<Atmos::Frame::StartCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Frame::StartCurator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Frame::StartCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::StartCurator>;
    };
}