#include "JavaScriptRenderCore.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"
#include "JavaScriptColor.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<RenderCore>::ToV8(v8::Isolate& isolate, const RenderCore& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.material.SetValue(isolate, value.material);
        definition.color.SetValue(isolate, value.color);
        return CreateObject(isolate, definition.ToAny());
    }

    Type<RenderCore>::Definition::Definition() :
        id("id"),
        material("material"),
        color("color")
    {}

    AnyObjectDefinition Type<RenderCore>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { material },
                { color }
            }
        };
    }

    auto Type<Arca::Index<Render::RenderCore>>::ToV8(v8::Isolate& isolate, const Arca::Index<Render::RenderCore>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        const auto material = value->material;
        return JavaScript::ToV8(
            isolate,
            RenderCore
            {
                value.ID(),
                material
                    ? MaterialAsset
                    {
                        material.ID(),
                        material->Name()
                    }
                    : std::optional<MaterialAsset>(),
                value->color
            });
    }

    std::optional<CreateRenderCore> Type<CreateRenderCore>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<CreateRenderCore, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto material = definition.material.AtmosValue();
                const auto color = definition.color.AtmosValue();
                return id && material && color
                    ? CreateRenderCore
                    {
                        *id, * material, * color
                    }
                    : std::optional<CreateRenderCore>{};
            });
    }

    Type<CreateRenderCore>::Definition::Definition() :
        id("id"),
        material("material"),
        color("color")
    {}

    AnyObjectDefinition Type<CreateRenderCore>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { material },
                { color }
            }
        };
    }

    std::optional<Arca::Create<Render::RenderCore>> Type<Arca::Create<Render::RenderCore>>::FromV8(
        v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));
        const auto reliquary = userData->reliquary;

        const auto command = JavaScript::FromV8<CreateRenderCore>(isolate, value);
        return command
            ? Arca::Create<Render::RenderCore>(
                command->id,
                command->material ? reliquary->Find<Asset::Material>(command->material->id) : Arca::Index<Asset::Material>(),
                command->color)
            : std::optional<Arca::Create<Render::RenderCore>>{};
    }
}