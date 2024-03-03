#include "AngelScriptReferenceCounted.h"

namespace Atmos::Scripting::Angel
{
    ReferenceCounted::ReferenceCounted() : refCount(1)
    {}

    ReferenceCounted::~ReferenceCounted() = default;

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