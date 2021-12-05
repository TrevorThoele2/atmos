#include "JavaScriptSymbol.h"

#include "JavaScriptString.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Symbol>::ToV8(v8::Isolate& isolate, const Symbol& value) -> v8::Local<V8T>
    {
        return v8::Symbol::New(
            &isolate,
            value.description ? JavaScript::ToV8(isolate, *value.description) : v8::Local<v8::String>());
    }

    std::optional<Symbol> Type<Symbol>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return value->IsSymbol()
            ? Symbol{ JavaScript::FromV8Required<String>(isolate, value.As<v8::Symbol>()->Description()) }
            : std::optional<Symbol>{};
    }
}