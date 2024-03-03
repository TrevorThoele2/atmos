#pragma once

#include "ArcaRelicIncludes.h"

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

        bool operator==(const Information& arg) const;
        bool operator!=(const Information& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Input::Information>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Input::Information";
        static constexpr Locality locality = Locality::Global;
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