#pragma once

#include <Arca/Curator.h>

#include "Work.h"

#include "WindowInformation.h"

namespace Atmos::Frame
{
    class IdleCurator : public Arca::Curator
    {
    public:
        explicit IdleCurator(Init init);

        void Handle(const Work& command);
    private:
        Arca::Index<Window::Information> windowInformation;

        [[nodiscard]] std::optional<Time::Nanoseconds> IdleFor();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::IdleCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Frame::IdleCurator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Frame::IdleCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Frame::IdleCurator>;
    };
}