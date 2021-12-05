#include "JavaScriptFindEntityByName.h"

#include "JavaScriptString.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Atmos::Entity::FindByName>::ToV8(v8::Isolate& isolate, const Entity::FindByName& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.name.SetValue(isolate, value.name);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Atmos::Entity::FindByName> Type<Atmos::Entity::FindByName>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Atmos::Entity::FindByName, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto name = definition.name.AtmosValue();
                return name
                    ? Atmos::Entity::FindByName
                    {
                        *name
                    }
                    : std::optional<Atmos::Entity::FindByName>{};
            });
    }

    Type<Atmos::Entity::FindByName>::Definition::Definition() :
        name("name")
    {}

    AnyObjectDefinition Type<Atmos::Entity::FindByName>::Definition::ToAny()
    {
        return
        {
            {
                { name }
            }
        };
    }
}