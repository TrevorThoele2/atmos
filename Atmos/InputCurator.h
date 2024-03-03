#pragma once

#include <Arca/Curator.h>

#include "MappedInputs.h"
#include "MappedActions.h"
#include "InputManager.h"
#include "Work.h"

namespace Atmos::Input
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, Manager& manager);

        void Handle(const Work& command);
    private:
        Manager* manager;

        Arca::Index<MappedInputs> mappedInputs;
        Arca::Index<MappedActions> mappedActions;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Input::Curator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Input::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Input::Curator>;
    };
}