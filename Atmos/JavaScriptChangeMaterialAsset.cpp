#include "JavaScriptChangeMaterialAsset.h"

#include "JavaScriptNumeric.h"
#include "JavaScriptOptional.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Render::ChangeMaterialAsset>::ToV8(v8::Isolate& isolate, const Render::ChangeMaterialAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.to.SetValue(isolate, value.to ? MaterialAsset(value.to.ID(), value.to->Name()) : std::optional<MaterialAsset>{});
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Render::ChangeMaterialAsset> Type<Render::ChangeMaterialAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Render::ChangeMaterialAsset, Definition>(
            isolate,
            value,
            [&isolate](const Definition& definition)
            {
                const auto userData = static_cast<UserData*>(isolate.GetData(0));
                const auto reliquary = userData->reliquary;

                const auto id = definition.id.AtmosValue();
                const auto to = definition.to.AtmosValue();
                return id && to
                    ? Render::ChangeMaterialAsset
                    {
                        *id, *to ? reliquary->Find<Asset::Material>((*to)->id) : Arca::Index<Asset::Material>()
                    }
                    : std::optional<Render::ChangeMaterialAsset>{};
            });
    }

    Type< Render::ChangeMaterialAsset>::Definition::Definition() :
        id("id"),
        to("to")
    {}

    AnyObjectDefinition Type<Render::ChangeMaterialAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { to }
            }
        };
    }
}