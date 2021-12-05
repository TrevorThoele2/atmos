#include "JavaScriptNameValuePair.h"

#include "JavaScriptString.h"
#include "JavaScriptSymbol.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<NameValuePair>::ToV8(v8::Isolate& isolate, const NameValuePair& value) -> v8::Local<V8T>
    {
        std::vector<v8::Local<v8::Name>> names =
        {
            JavaScript::ToV8(isolate, Symbol{}),
            JavaScript::ToV8(isolate, Symbol{})
        };

        std::vector<v8::Local<v8::Value>> values =
        {
            JavaScript::ToV8(isolate, value.name),
            JavaScript::ToV8(isolate, value.value)
        };

        return v8::Object::New(&isolate, v8::Null(&isolate), names.data(), values.data(), 1);
    }

    std::optional<NameValuePair> Type<NameValuePair>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto context = isolate.GetCurrentContext();

        v8::Local<v8::Object> object;
        if (!value->ToObject(context).ToLocal(&object))
            return std::optional<NameValuePair>{};

        const auto name = JavaScript::FromV8<String>(
            isolate, object->Get(context, JavaScript::ToV8(isolate, String("name"))));
        const auto v8Value = JavaScript::FromV8<String>(
            isolate, object->Get(context, JavaScript::ToV8(isolate, String("value"))));
        return name && v8Value ? NameValuePair{ *name, *v8Value } : std::optional<NameValuePair>{};
    }
}