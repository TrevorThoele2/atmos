#pragma once

#include "Serialization.h"

namespace Atmos
{
    class Scalers3D
    {
    public:
        typedef float Value;
    public:
        Value x;
        Value y;
        Value z;
    public:
        Scalers3D();
        Scalers3D(Value x, Value y, Value z);
        Scalers3D(const Scalers3D& arg) = default;

        Scalers3D& operator=(const Scalers3D& arg);

        bool operator==(const Scalers3D& arg) const;
        bool operator!=(const Scalers3D& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Scalers3D, BinaryArchive> : public CompositeScribe<::Atmos::Scalers3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}