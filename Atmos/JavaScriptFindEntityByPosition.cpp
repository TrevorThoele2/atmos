#include "JavaScriptFindEntityByPosition.h"

#include "JavaScriptGridPoint.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Atmos::Entity::FindByPosition>::ToV8(v8::Isolate& isolate, const Atmos::Entity::FindByPosition& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.position.SetValue(isolate, value.position);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Atmos::Entity::FindByPosition> Type<Atmos::Entity::FindByPosition>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Atmos::Entity::FindByPosition, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto position = definition.position.AtmosValue();
                return position
                    ? Atmos::Entity::FindByPosition
                    {
                        *position
                    }
                    : std::optional<Atmos::Entity::FindByPosition>{};
            });
    }

    Type<Atmos::Entity::FindByPosition>::Definition::Definition() :
        position("position")
    {}

    AnyObjectDefinition Type<Atmos::Entity::FindByPosition>::Definition::ToAny()
    {
        return
        {
            {
                { position }
            }
        };
    }
}