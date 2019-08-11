#include "ArchiveBase.h"

#include <Inscription/BinaryArchive.h>

namespace Atmos::World::Serialization
{
    ArchiveBase::~ArchiveBase()
    {}

    void ArchiveBase::CopyTrackersTo(ArchiveBase& archive) const
    {
        UnderlyingArchive().CopyTrackersTo(archive.UnderlyingArchive());
    }

    void ArchiveBase::CopyTrackersTo(::Inscription::BinaryArchive& archive) const
    {
        UnderlyingArchive().CopyTrackersTo(archive);
    }

    void ArchiveBase::MoveTrackersTo(ArchiveBase& archive)
    {
        UnderlyingArchive().MoveTrackersTo(archive.UnderlyingArchive());
    }

    void ArchiveBase::MoveTrackersTo(::Inscription::BinaryArchive& archive)
    {
        UnderlyingArchive().MoveTrackersTo(archive);
    }
}