#include "JavaScriptString.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<String>::ToV8(v8::Isolate& isolate, const String& value) -> v8::Local<V8T>
    {
        return v8::String::NewFromUtf8(&isolate, value.c_str()).ToLocalChecked();
    }

    std::optional<String> Type<String>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        v8::Local<v8::String> v8Value;
        return value->IsString() && value->ToString(isolate.GetCurrentContext()).ToLocal(&v8Value)
            ? String(*v8::String::Utf8Value(&isolate, v8Value))
            : std::optional<String>{};
    }
}