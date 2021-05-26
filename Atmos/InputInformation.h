#pragma once

#include <Arca/Relic.h>

#include "Point2D.h"

#include "Delta.h"

namespace Atmos::Input
{
    class Information
    {
    public:
        Delta<Spatial::Point2D> mousePosition;
    public:
        Information() = default;
        Information(Information&& arg) noexcept = default;

        Information& operator=(Information&& arg) noexcept;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Input::Information"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Input::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Input::Information>;
    };
}