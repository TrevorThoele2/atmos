#pragma once

#include "ScriptError.h"

namespace Atmos::Scripting::Angel
{
    [[nodiscard]] bool IsSuccess(int result);
    [[nodiscard]] bool IsError(int result);
    void VerifyResult(int result, const Exception::NameValuePairs& details);
}