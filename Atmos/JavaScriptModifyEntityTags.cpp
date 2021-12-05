#include "JavaScriptModifyEntityTags.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptString.h"
#include "JavaScriptVector.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Atmos::Entity::ModifyTags>::ToV8(v8::Isolate& isolate, const Atmos::Entity::ModifyTags& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.entity.SetValue(isolate, value.entity);
        definition.add.SetValue(isolate, value.add);
        definition.remove.SetValue(isolate, value.remove);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Atmos::Entity::ModifyTags> Type<Atmos::Entity::ModifyTags>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Atmos::Entity::ModifyTags, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto entity = definition.entity.AtmosValue();
                const auto add = definition.add.AtmosValue();
                const auto remove = definition.remove.AtmosValue();
                return entity && add && remove
                    ? Atmos::Entity::ModifyTags
                    {
                        *entity, *add, *remove
                    }
                    : std::optional<Atmos::Entity::ModifyTags>{};
            });
    }

    Type<Atmos::Entity::ModifyTags>::Definition::Definition() :
        entity("entity"),
        add("add"),
        remove("remove")
    {}

    AnyObjectDefinition Type<Atmos::Entity::ModifyTags>::Definition::ToAny()
    {
        return
        {
            {
                { entity },
                { add },
                { remove }
            }
        };
    }
}