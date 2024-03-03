#include "JavaScriptSpace.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptEnum.h"

namespace Atmos::Scripting::JavaScript
{
    v8::Local<v8::Value> Type<Spatial::Space>::Register(const RegistrationData& data)
    {
        const auto isolate = data.isolate;

        return CreateEnum<Spatial::Space>(
            *isolate,
            {
                { "World", Spatial::Space::World },
                { "Screen", Spatial::Space::Screen }
            });
    }

    auto Type<Spatial::Space>::ToV8(v8::Isolate& isolate, const Spatial::Space& value) -> v8::Local<V8T>
    {
        return EnumType<Spatial::Space>::ToV8(isolate, value);
    }

    std::optional<Spatial::Space> Type<Spatial::Space>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return EnumType<Spatial::Space>::FromV8(isolate, value);
    }
}