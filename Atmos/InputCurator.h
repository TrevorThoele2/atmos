#pragma once

#include <Arca/Curator.h>

#include "MappedInputs.h"
#include "MappedActions.h"
#include "InputManager.h"

namespace Atmos::Input
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, Manager& manager);

        void Work();
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
        static inline const TypeName typeName = "InputCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Input::Curator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Input::Curator, BinaryArchive>
    {};
}