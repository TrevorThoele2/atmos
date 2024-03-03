#pragma once

#include "AngelScriptReferenceCounted.h"

#include "ObjectID.h"

namespace Atmos::Scripting
{
    class Object : public ReferenceCounted
    {
    public:
        ObjectID id;

        virtual ~Object() = 0;
    };
}