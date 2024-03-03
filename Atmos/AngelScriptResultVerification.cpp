#include <cstdlib>

#include "AngelScriptResultVerification.h"

#include <Chroma/Contract.h>

namespace Atmos::Script::Angel
{
    void VerifyResult(int result)
    {
        SOFT_PRECONDITION(result >= 0, AngelScriptFailed());
    }

    AngelScriptFailed::AngelScriptFailed() : Exception("A method from AngelScript has failed verification.")
    {}
}