#pragma once

#include <Arca/Relic.h>
#include "WindowBase.h"

namespace Atmos::Window
{
    class Information final
    {
    public:
        WindowBase* window;

        explicit Information(WindowBase& window);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Window::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Window::Information";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Window::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Window::Information>;
    };
}