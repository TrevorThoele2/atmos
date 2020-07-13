#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Size2D
    {
        using Value = float;
        Value width = 0.0f;
        Value height = 0.0f;

        bool operator==(const Size2D& arg) const;
        bool operator!=(const Size2D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Spatial::Size2D, BinaryArchive> final :
        public CompositeScribe<::Atmos::Spatial::Size2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}