#pragma once

#include "Exception.h"

namespace Atmos::Scripting::Angel
{
    [[nodiscard]] bool IsSuccess(int result);
    [[nodiscard]] bool IsError(int result);
    void VerifyResult(int result);

    class AngelScriptFailed final : public Exception
    {
    public:
        AngelScriptFailed();
        AngelScriptFailed(const String& message);
        AngelScriptFailed(const String& message, const NameValuePairs& details);
    private:
        static String BaseMessage();
    };
}