#include "JavaScriptImageAssetGridSize.h"

#include "JavaScriptNumeric.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    auto Type<Asset::ImageGridSize>::ToV8(v8::Isolate& isolate, const Asset::ImageGridSize& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.columns.SetValue(isolate, value.columns);
        definition.rows.SetValue(isolate, value.rows);
        return CreateObject(isolate, definition.ToAny());
    }

    std::optional<Asset::ImageGridSize> Type<Asset::ImageGridSize>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Asset::ImageGridSize, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto columns = definition.columns.AtmosValue();
                const auto rows = definition.rows.AtmosValue();
                return columns && rows
                    ? Asset::ImageGridSize
                    {
                        *columns, *rows
                    }
                    : std::optional<Asset::ImageGridSize>{};
            });
    }

    Type<Asset::ImageGridSize>::Definition::Definition() :
        columns("columns"),
        rows("rows")
    {}

    AnyObjectDefinition Type<Asset::ImageGridSize>::Definition::ToAny()
    {
        return
        {
            {
                { columns },
                { rows }
            }
        };
    }
}