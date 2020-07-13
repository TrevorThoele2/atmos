#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Scalers2D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;

        bool operator==(const Scalers2D& arg) const;
        bool operator!=(const Scalers2D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Spatial::Scalers2D, BinaryArchive> final :
        public CompositeScribe<::Atmos::Spatial::Scalers2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}