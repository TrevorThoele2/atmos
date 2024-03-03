#pragma once

#include "JavaScriptType.h"
#include "RequestField.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<World::RequestField>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const World::RequestField& value);
        [[nodiscard]] static std::optional<World::RequestField> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<World::FieldID> id;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}