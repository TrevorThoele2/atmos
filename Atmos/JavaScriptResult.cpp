#include "JavaScriptResult.h"

#include "JavaScriptVariant.h"
#include "JavaScriptString.h"
#include "JavaScriptBool.h"
#include "JavaScriptVector.h"
#include "JavaScriptSymbol.h"
#include "JavaScriptOptional.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Result>::ToV8(v8::Isolate& isolate, const Result& value) -> v8::Local<V8T>
    {
        std::vector<v8::Local<v8::Name>> names =
        {
            JavaScript::ToV8(isolate, Symbol{}),
            JavaScript::ToV8(isolate, Symbol{})
        };

        std::vector<v8::Local<v8::Value>> values =
        {
            JavaScript::ToV8(isolate, false),
            JavaScript::ToV8(isolate, std::optional<Variant>{})
        };

        return v8::Object::New(&isolate, v8::Null(&isolate), names.data(), values.data(), 1);
    }

    std::optional<Result> Type<Result>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto context = isolate.GetCurrentContext();

        v8::Local<v8::Object> object;
        if (!value->ToObject(context).ToLocal(&object))
            return std::optional<Result>{};

        const auto done = JavaScript::FromV8<bool>(
            isolate, object->Get(context, JavaScript::ToV8(isolate, String("done"))));
        const auto variantValue = *JavaScript::FromV8<std::optional<Variant>>(
            isolate, object->Get(context, JavaScript::ToV8(isolate, String("value"))));
        return done ? Result{ *done, variantValue } : std::optional<Result>{};
    }
}