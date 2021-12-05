#include "JavaScriptMoveEntityTo.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptGridPoint.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Atmos::Entity::MoveTo>::ToV8(v8::Isolate& isolate, const Entity::MoveTo& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.entity.SetValue(isolate, value.entity);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Atmos::Entity::MoveTo> Type<Atmos::Entity::MoveTo>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Atmos::Entity::MoveTo, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto entity = definition.entity.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return entity && to
                    ? Atmos::Entity::MoveTo
                    {
                        *entity, *to
                    }
                    : std::optional<Atmos::Entity::MoveTo>{};
            });
    }

    Type<Atmos::Entity::MoveTo>::Definition::Definition() :
        entity("entity"),
        to("to")
    {}

    AnyObjectDefinition Type<Atmos::Entity::MoveTo>::Definition::ToAny()
    {
        return
        {
            {
                { entity },
                { to }
            }
        };
    }
}