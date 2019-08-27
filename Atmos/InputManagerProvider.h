#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "UniqueProvider.h"
#include "InputManager.h"

namespace Atmos::Input
{
    class ManagerProvider final
        : public Arca::ClosedTypedRelicAutomation<ManagerProvider>
        , public UniqueProvider<Manager>
    {
    public:
        ManagerProvider();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::ManagerProvider>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Input::ManagerProvider, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Input::ManagerProvider, BinaryArchive>
    {};
}