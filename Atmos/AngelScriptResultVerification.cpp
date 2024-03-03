#include "AngelScriptResultVerification.h"

namespace Atmos::Scripting::Angel
{
    bool IsSuccess(int result)
    {
        return result >= 0;
    }

    bool IsError(int result)
    {
        return result < 0;
    }

    void VerifyResult(int result)
    {
        if (IsError(result))
            throw AngelScriptFailed();
    }

    AngelScriptFailed::AngelScriptFailed() :
        Exception(BaseMessage())
    {}

    AngelScriptFailed::AngelScriptFailed(const String& message) :
        Exception(message)
    {}

    AngelScriptFailed::AngelScriptFailed(const String& message, const NameValuePairs& details) :
        Exception(message, details)
    {}

    String AngelScriptFailed::BaseMessage()
    {
        return "A method from AngelScript has failed verification.";
    }
}