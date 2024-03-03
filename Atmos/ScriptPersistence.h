#pragma once

#include "Name.h"

#include "Serialization.h"

namespace Atmos::Scripting
{
    class Persistence
    {
    public:
        typedef size_t SizeT;
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
    class Scribe<::Atmos::Scripting::Persistence, BinaryArchive> :
        public CompositeScribe<::Atmos::Scripting::Persistence, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}