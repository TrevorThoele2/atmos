#pragma once

#include "AngelScriptReferenceCounted.h"

#include <Arca/RelicID.h>

namespace Atmos::Script::Angel
{
    class Relic : public ReferenceCounted
    {
    public:
        Arca::RelicID id;

        virtual ~Relic() = 0;
    };
}