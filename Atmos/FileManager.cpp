#include "FileManager.h"

namespace Atmos::File
{
    FileManager::~FileManager()
    {}
}

namespace Inscription
{
    void Scribe<::Atmos::File::FileManager, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}