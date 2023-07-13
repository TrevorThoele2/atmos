#include "JavaScriptFontAsset.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<FontAsset>::ToV8(v8::Isolate& isolate, const FontAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<FontAsset> Type<FontAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<FontAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                return id && name
                    ? FontAsset
                    {
                        *id, *name
                    }
                    : std::optional<FontAsset>{};
            });
    }

    Type<FontAsset>::Definition::Definition() :
        id("id"),
        name("name")
    {}

    AnyObjectDefinition Type<FontAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name }
            }
        };
    }

    auto Type<Arca::Index<Asset::Font>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Font>& value) -> v8::Local<V8T>
    {
        if (!value)
            return v8::Null(&isolate);

        return JavaScript::ToV8(
            isolate,
            FontAsset
            {
                value.ID(),
                value->Name()
            });
    }
}