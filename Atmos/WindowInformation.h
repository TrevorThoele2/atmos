#pragma once

#include "ArcaRelicIncludes.h"
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
        static TypeName TypeName() { return "Atmos::Window::Information"; }
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