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

        [[nodiscard]] bool IsDone();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Frame::IdleCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline TypeName typeName = "Atmos::Frame::IdleCurator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}