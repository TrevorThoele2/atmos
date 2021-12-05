#include "JavaScriptRequestField.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<World::RequestField>::ToV8(v8::Isolate& isolate, const World::RequestField& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<World::RequestField> Type<World::RequestField>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<World::RequestField, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                return id
                    ? World::RequestField
                    {
                        *id
                    }
                    : std::optional<World::RequestField>{};
                });
    }

    Type<World::RequestField>::Definition::Definition() :
        id("id")
    {}

    AnyObjectDefinition Type<World::RequestField>::Definition::ToAny()
    {
        return
        {
            {
                { id }
            }
        };
    }
}