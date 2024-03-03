
#include "ScribeBase.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    void ScribeBase::CopyTrackersTo(ScribeBase &arg) const
    {
        GetBasicScribe().CopyTrackersTo(arg.GetBasicScribe());
    }

    void ScribeBase::CopyTrackersTo(inscription::Scribe &scribe) const
    {
        GetBasicScribe().CopyTrackersTo(scribe);
    }

    void ScribeBase::MoveTrackersTo(ScribeBase &arg)
    {
        GetBasicScribe().MoveTrackersTo(arg.GetBasicScribe());
    }

    void ScribeBase::MoveTrackersTo(inscription::Scribe &scribe)
    {
        GetBasicScribe().MoveTrackersTo(scribe);
    }
}