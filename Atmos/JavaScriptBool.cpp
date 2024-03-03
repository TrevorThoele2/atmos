#include "JavaScriptBool.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<bool>::ToV8(v8::Isolate& isolate, const bool& value) -> v8::Local<V8T>
    {
        return v8::Boolean::New(&isolate, value);
    }

    std::optional<bool> Type<bool>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return value->IsBoolean() ? value->BooleanValue(&isolate) : std::optional<bool>{};
    }
}