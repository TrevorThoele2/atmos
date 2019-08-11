#pragma once

#include "Serialization.h"

namespace Atmos
{
    class Scalers2D
    {
    public:
        typedef float Value;
    public:
        Value x;
        Value y;
    public:
        Scalers2D();
        Scalers2D(Value x, Value y);
        Scalers2D(const Scalers2D& arg) = default;

        Scalers2D& operator=(const Scalers2D& arg);

        bool operator==(const Scalers2D& arg) const;
        bool operator!=(const Scalers2D& arg) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Scalers2D, BinaryArchive> :
        public CompositeScribe<::Atmos::Scalers2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}