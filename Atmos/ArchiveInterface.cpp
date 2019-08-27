#include "ArchiveInterface.h"

#include <Inscription/BinaryArchive.h>
#include <utility>

namespace Atmos::World::Serialization
{
    ArchiveInterface::~ArchiveInterface() = default;

    void ArchiveInterface::CopyTrackersTo(ArchiveInterface& archive) const
    {
        Archive().CopyTrackersTo(archive.Archive());
    }

    void ArchiveInterface::CopyTrackersTo(::Inscription::BinaryArchive& archive) const
    {
        Archive().CopyTrackersTo(archive);
    }

    void ArchiveInterface::MoveTrackersTo(ArchiveInterface& archive)
    {
        Archive().MoveTrackersTo(archive.Archive());
    }

    void ArchiveInterface::MoveTrackersTo(::Inscription::BinaryArchive& archive)
    {
        Archive().MoveTrackersTo(archive);
    }

    File::Path ArchiveInterface::FilePath() const
    {
        return filePath;
    }

    ArchiveInterface::ArchiveInterface(File::Path filePath) : filePath(std::move(filePath))
    {}
}