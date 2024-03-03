#pragma once

#include "Name.h"

#include "Serialization.h"

namespace Atmos::Script
{
    class Persistence
    {
    public:
        using SizeT = size_t;
    private:
        class Impl;
    public:
        Persistence();
        Persistence(const Persistence& arg);
        ~Persistence();

        Persistence& operator=(const Persistence& arg);

        SizeT Size() const;
        bool IsEmpty() const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Script::Persistence, BinaryArchive> :
        public CompositeScribe<::Atmos::Script::Persistence, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}