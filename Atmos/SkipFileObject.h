
#pragma once

#include <Inscription\Scribe.h>

namespace Atmos
{
    template<class ScribeT>
    class SkipFileObject
    {
    public:
        typedef ScribeT FocusedScribe;
    public:
        FocusedScribe& scribe;
    public:
        SkipFileObject(FocusedScribe& scribe);
    protected:
        typedef typename FocusedScribe::StreamPosition Position;
    protected:
        Position position;
    };

    template<class ScribeT>
    SkipFileObject<ScribeT>::SkipFileObject(FocusedScribe& scribe) : scribe(scribe), position(0)
    {}
}