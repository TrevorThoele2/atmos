
#include <cstdlib>

#include "AngelScriptAssert.h"

#include "Assert.h"

namespace Atmos::Scripting
{
    void AngelScriptAssert(int result)
    {
        ATMOS_ASSERT(result >= 0);
    }
}