#pragma once

namespace Inscription
{
    class BinaryArchive;
}

namespace Atmos::World::Serialization
{
    class ArchiveBase
    {
    public:
        virtual ~ArchiveBase() = 0;

        void CopyTrackersTo(ArchiveBase& archive) const;
        void CopyTrackersTo(::Inscription::BinaryArchive& archive) const;
        void MoveTrackersTo(ArchiveBase& archive);
        void MoveTrackersTo(::Inscription::BinaryArchive& archive);
    protected:
        ArchiveBase() = default;
    protected:
        virtual ::Inscription::BinaryArchive& UnderlyingArchive() = 0;
        virtual const ::Inscription::BinaryArchive& UnderlyingArchive() const = 0;
    };
}