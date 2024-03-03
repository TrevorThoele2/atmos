#include <unordered_map>

#include "ScriptPersistence.h"

#include "Optional.h"

namespace Atmos::Script
{
    Persistence::Persistence()
    {}

    Persistence::Persistence(const Persistence& arg)
    {}

    Persistence::~Persistence()
    {}

    Persistence& Persistence::operator=(const Persistence& arg)
    {
        return *this;
    }

    Persistence::SizeT Persistence::Size() const
    {
        return 0;
    }

    bool Persistence::IsEmpty() const
    {
        return true;
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Script::Persistence, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {}

    void Scribe<::Atmos::Script::Persistence, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}