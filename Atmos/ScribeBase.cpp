
#include "ScribeBase.h"

#include <Inscription/BinaryScribe.h>

namespace Atmos
{
    ScribeBase::~ScribeBase()
    {}

    void ScribeBase::CopyTrackersTo(ScribeBase& scribe) const
    {
        UnderlyingScribe().CopyTrackersTo(scribe.UnderlyingScribe());
    }

    void ScribeBase::CopyTrackersTo(::Inscription::BinaryScribe& scribe) const
    {
        UnderlyingScribe().CopyTrackersTo(scribe);
    }

    void ScribeBase::MoveTrackersTo(ScribeBase& scribe)
    {
        UnderlyingScribe().MoveTrackersTo(scribe.UnderlyingScribe());
    }

    void ScribeBase::MoveTrackersTo(::Inscription::BinaryScribe& scribe)
    {
        UnderlyingScribe().MoveTrackersTo(scribe);
    }
}