#include "AssetCore.h"

namespace Atmos::Asset
{
    Core::Core(const Name& name) : name(name)
    {}
}

namespace Inscription
{
    void Scribe<Atmos::Asset::Core, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.name);
    }
}