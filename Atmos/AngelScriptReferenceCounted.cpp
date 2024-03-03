
#include "AngelScriptReferenceCounted.h"

namespace Atmos::Script
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