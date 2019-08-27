#pragma once

#include "FilePath.h"
#include <Inscription/BinaryArchive.h>

namespace Atmos::World::Serialization
{
    class ArchiveInterface
    {
    public:
        virtual ~ArchiveInterface() = 0;

        void CopyTrackersTo(ArchiveInterface& archive) const;
        void CopyTrackersTo(::Inscription::BinaryArchive& archive) const;
        void MoveTrackersTo(ArchiveInterface& archive);
        void MoveTrackersTo(::Inscription::BinaryArchive& archive);

        [[nodiscard]] File::Path FilePath() const;
    protected:
        explicit ArchiveInterface(File::Path filePath);
    protected:
        [[nodiscard]] virtual ::Inscription::BinaryArchive& Archive() = 0;
        [[nodiscard]] virtual const ::Inscription::BinaryArchive& Archive() const = 0;
    private:
        File::Path filePath;
    };
}