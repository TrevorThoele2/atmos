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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Window::Information";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Window::Information, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Window::Information>;
    };
}