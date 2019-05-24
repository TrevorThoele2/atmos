#pragma once

namespace Inscription { class BinaryScribe; }

namespace Atmos
{
    class ScribeBase
    {
    public:
        virtual ~ScribeBase() = 0;

        void CopyTrackersTo(ScribeBase& scribe) const;
        void CopyTrackersTo(::Inscription::BinaryScribe& scribe) const;
        void MoveTrackersTo(ScribeBase& scribe);
        void MoveTrackersTo(::Inscription::BinaryScribe& scribe);
    protected:
        ScribeBase() = default;
    protected:
        virtual ::Inscription::BinaryScribe& UnderlyingScribe() = 0;
        virtual const ::Inscription::BinaryScribe& UnderlyingScribe() const = 0;
    };
}