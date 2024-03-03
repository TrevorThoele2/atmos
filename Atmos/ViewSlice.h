#pragma once

#include "ArcaShardIncludes.h"

#include "AxisAlignedBox2D.h"

namespace Atmos::Render
{
    struct ViewSlice
    {
        Spatial::AxisAlignedBox2D box;

        ViewSlice() = default;
        ViewSlice(const Spatial::AxisAlignedBox2D& box);

        bool operator==(const ViewSlice& arg) const;
        bool operator!=(const ViewSlice& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ViewSlice>
    {
        static constexpr ObjectType objectType = ObjectType::Shard;
        static const inline TypeName typeName = "Atmos::Render::ViewSlice";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::ViewSlice> final
    {
    public:
        using ObjectT = Atmos::Render::ViewSlice;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("box", object.box);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::ViewSlice, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::ViewSlice>;
    };
}