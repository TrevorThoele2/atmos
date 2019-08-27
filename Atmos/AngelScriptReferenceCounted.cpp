#include "AngelScriptReferenceCounted.h"

namespace Atmos::Script::Angel
{
    ReferenceCounted::ReferenceCounted() : refCount(1)
    {}

    ReferenceCounted::~ReferenceCounted()
    {}

    void ReferenceCounted::AddRef()
    {
        ++refCount;
    }

    void ReferenceCounted::Release()
    {
        if (--refCount == 0)
            delete this;
    }
}