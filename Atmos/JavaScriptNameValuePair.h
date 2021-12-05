#pragma once

#include "JavaScriptType.h"
#include "NameValuePair.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<NameValuePair>
    {
    public:
        using V8T = v8::Object;
        
        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const NameValuePair& value);
        [[nodiscard]] static std::optional<NameValuePair> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}