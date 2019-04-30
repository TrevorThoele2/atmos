
#include "ScribeBase.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    ScribeBase::~ScribeBase()
    {}

    void ScribeBase::CopyTrackersTo(ScribeBase& scribe) const
    {
        UnderlyingScribe().CopyTrackersTo(scribe.UnderlyingScribe());
    }

    void ScribeBase::CopyTrackersTo(::Inscription::Scribe& scribe) const
    {
        UnderlyingScribe().CopyTrackersTo(scribe);
    }

    void ScribeBase::MoveTrackersTo(ScribeBase& scribe)
    {
        UnderlyingScribe().MoveTrackersTo(scribe.UnderlyingScribe());
    }

    void ScribeBase::MoveTrackersTo(::Inscription::Scribe& scribe)
    {
        UnderlyingScribe().MoveTrackersTo(scribe);
    }
}