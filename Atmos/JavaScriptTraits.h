#pragma once

#include "JavaScriptType.h"
#include "Log.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    struct ArcaTraits
    {
        String typeName;

        template<class T>
        static ArcaTraits From();
    };

    template<class T>
	ArcaTraits ArcaTraits::From()
    {
        return
        {
            Arca::Traits<T>::typeName
        };
    }

    template<>
    class Type<ArcaTraits>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<v8::Value> Register(const RegistrationData& data);

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const ArcaTraits& value);
        [[nodiscard]] static std::optional<ArcaTraits> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> typeName;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}