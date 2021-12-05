#include "JavaScriptImageAsset.h"

#include "JavaScriptImageAssetGridSize.h"
#include "JavaScriptSize2D.h"
#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"
#include "JavaScriptUserData.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<ImageAsset>::ToV8(v8::Isolate& isolate, const ImageAsset& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.id.SetValue(isolate, value.id);
        definition.name.SetValue(isolate, value.name);
        definition.size.SetValue(isolate, value.size);
        definition.gridSize.SetValue(isolate, value.gridSize);
        definition.sliceSize.SetValue(isolate, value.sliceSize);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<ImageAsset> Type<ImageAsset>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<ImageAsset, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto id = definition.id.AtmosValue();
                const auto name = definition.name.AtmosValue();
                const auto size = definition.size.AtmosValue();
                const auto gridSize = definition.gridSize.AtmosValue();
                const auto sliceSize = definition.sliceSize.AtmosValue();
                return id && name && size && gridSize && sliceSize
                    ? ImageAsset
                    {
                        *id, *name, *size, *gridSize, *sliceSize
                    }
                    : std::optional<ImageAsset>{};
            });
    }

    Type<ImageAsset>::Definition::Definition() :
        id("id"),
        name("name"),
        size("size"),
        gridSize("gridSize"),
        sliceSize("sliceSize")
    {}

    AnyObjectDefinition Type<ImageAsset>::Definition::ToAny()
    {
        return
        {
            {
                { id },
                { name },
                { size },
                { gridSize },
                { sliceSize }
            }
        };
    }

    auto Type<Arca::Index<Asset::Image>>::ToV8(v8::Isolate& isolate, const Arca::Index<Asset::Image>& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(
            isolate,
            ImageAsset
            {
                value.ID(),
                value->Name(),
                value->Size(),
                value->GridSize(),
                value->SliceSize()
            });
    }

    std::optional<Arca::Index<Asset::Image>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto userData = static_cast<UserData*>(isolate.GetData(0));

        const auto from = JavaScript::FromV8<ImageAsset>(isolate, value);
        return from ? userData->reliquary->Find<Asset::Image>(from->id) : std::optional<Arca::Index<Asset::Image>>{};
    }
}