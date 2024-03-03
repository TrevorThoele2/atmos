#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Size3D
    {
        using Value = float;
        Value width = 0.0f;
        Value height = 0.0f;
        Value depth = 0.0f;

        bool operator==(const Size3D& arg) const;
        bool operator!=(const Size3D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Size3D, BinaryArchive> final :
        public CompositeScribe<Atmos::Spatial::Size3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}