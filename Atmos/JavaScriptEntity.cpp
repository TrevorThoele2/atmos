#include "JavaScriptEntity.h"

#include "JavaScriptBool.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptVector.h"
#include "JavaScriptGridPoint.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Entity>::ToV8(v8::Isolate& isolate, const Entity& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        definition.position.SetValue(isolate, value.position);
        definition.isSolid.SetValue(isolate, value.isSolid);
        definition.tags.SetValue(isolate, value.tags);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Entity> Type<Entity>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Entity, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto isSolid = definition.isSolid.AtmosValue();
                const auto tags = definition.tags.AtmosValue();
                return id && name && position && isSolid && tags
                    ? Entity
                    {
                        *id, *name, *position, *isSolid, *tags
                    }
                    : std::optional<Entity>{};
            });
    }

    Type<Entity>::Definition::Definition() :
        id("id"),
        name("name"),
        position("position"),
        isSolid("isSolid"),
        tags("tags")
    {}

    AnyObjectDefinition Type<Entity>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name },
                { position },
                { isSolid },
                { tags }
            }
        };
    }

    auto Type<Arca::Index<Atmos::Entity::Entity>>::ToV8(v8::Isolate& isolate, const Arca::Index<Atmos::Entity::Entity>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            Entity
            {
                value.ID(),
                value->name,
                value->position,
                value->isSolid,
                value->tags
            });
    }

    std::optional<CreateEntity> Type<CreateEntity>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateEntity, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto name = definition.name.AtmosValue();
                const auto position = definition.position.AtmosValue();
                const auto isSolid = definition.isSolid.AtmosValue();
                const auto tags = definition.tags.AtmosValue();
                return name && position && isSolid && tags
                    ? CreateEntity
                    {
                        *name, *position, *isSolid, *tags
                    }
                    : std::optional<CreateEntity>{};
            });
    }

    Type<CreateEntity>::Definition::Definition() :
        name("name"),
        position("position"),
        isSolid("isSolid"),
        tags("tags")
    {}

    AnyObjectDefinition Type<CreateEntity>::Definition::ToAny()
    {
        return
        {
            {
                { name },
                { position },
                { isSolid },
                { tags }
            }
        };
    }

    std::optional<Arca::Create<Atmos::Entity::Entity>> Type<Arca::Create<Atmos::Entity::Entity>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateEntity>(isolate, value);
        return command
            ? Arca::Create<Atmos::Entity::Entity>(
                command->name,
                command->position,
                command->isSolid,
                command->tags)
            : std::optional<Arca::Create<Atmos::Entity::Entity>>{};
    }
}