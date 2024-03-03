#pragma once

#include <v8.h>
#include "ScriptResult.h"

namespace Atmos::Scripting::JavaScript
{
    [[nodiscard]] Failure ToFailure(
        v8::Isolate& isolate, const v8::Local<v8::Context>& context, const v8::Local<v8::Message>& message);
}