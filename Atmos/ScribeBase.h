#pragma once

namespace Inscription { class Scribe; }

namespace Atmos
{
    class ScribeBase
    {
    public:
        virtual ~ScribeBase() = 0;

        void CopyTrackersTo(ScribeBase& scribe) const;
        void CopyTrackersTo(::Inscription::Scribe& scribe) const;
        void MoveTrackersTo(ScribeBase& scribe);
        void MoveTrackersTo(::Inscription::Scribe& scribe);
    protected:
        ScribeBase() = default;
    protected:
        virtual ::Inscription::Scribe& UnderlyingScribe() = 0;
        virtual const ::Inscription::Scribe& UnderlyingScribe() const = 0;
    };
}