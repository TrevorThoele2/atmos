#include "JavaScriptVariant.h"

#include "JavaScriptBool.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptString.h"
#include "ScriptError.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    std::optional<Variant> FromV8Variant(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto fromValue = FromV8<T>(isolate, value);
        return fromValue ? *fromValue : std::optional<Variant>{};
    }

    template<class T>
    std::optional<Variant> FromV8Numeric(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto fromValue = FromV8<T>(isolate, value);
        return fromValue ? static_cast<T>(*fromValue) : std::optional<T>{};
    }

    auto Type<Variant>::ToV8(v8::Isolate& isolate, const Variant& value) -> v8::Local<V8T>
    {
        return std::visit(
            [&isolate](auto value) -> v8::Local<v8::Value>
            {
                return JavaScript::ToV8(isolate, value);
            },
            value);
    }

    std::optional<Variant> Type<Variant>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        if (value->IsBoolean())
            return FromV8Variant<bool>(isolate, value);
        else if (value->IsInt32())
            return FromV8Numeric<std::int32_t>(isolate, value);
        else if (value->IsBigInt())
            return FromV8Numeric<std::int64_t>(isolate, value);
        else if (value->IsNumber())
            return FromV8Numeric<double>(isolate, value);
        else if (value->IsString())
            return FromV8Variant<String>(isolate, value);
        else
            return {};
    }
}