#include "JavaScriptCanMoveEntityTo.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptGridPoint.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Atmos::Entity::CanMoveTo>::ToV8(v8::Isolate& isolate, const Atmos::Entity::CanMoveTo& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.entity.SetValue(isolate, value.entity);
        definition.to.SetValue(isolate, value.to);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Atmos::Entity::CanMoveTo> Type<Atmos::Entity::CanMoveTo>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Atmos::Entity::CanMoveTo, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto entity = definition.entity.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return entity && to
                    ? Atmos::Entity::CanMoveTo
                    {
                        *entity, *to
                    }
                    : std::optional<Atmos::Entity::CanMoveTo>{};
            });
    }

    Type<Atmos::Entity::CanMoveTo>::Definition::Definition() :
        entity("entity"),
        to("to")
    {}

    AnyObjectDefinition Type<Atmos::Entity::CanMoveTo>::Definition::ToAny()
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