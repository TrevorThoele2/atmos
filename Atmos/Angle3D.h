#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Angle3D
    {
        using Value = float;

        Value yaw;
        Value pitch;

        [[nodiscard]] bool operator==(const Angle3D& arg) const;
        [[nodiscard]] bool operator!=(const Angle3D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Angle3D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Angle3D;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("yaw", object.yaw);
            archive("pitch", object.pitch);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Angle3D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Angle3D>;
    };
}