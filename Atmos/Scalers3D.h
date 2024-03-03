#pragma once

#include "Serialization.h"

namespace Atmos
{
    struct Scalers3D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;
        Value z = 0.0f;

        bool operator==(const Scalers3D& arg) const;
        bool operator!=(const Scalers3D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Scalers3D, BinaryArchive> final : public CompositeScribe<::Atmos::Scalers3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}