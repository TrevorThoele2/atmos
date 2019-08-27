#pragma once

#include "Exception.h"

namespace Atmos::Script::Angel
{
    void VerifyResult(int result);

    class AngelScriptFailed : public Exception
    {
    public:
        AngelScriptFailed();
    };
}