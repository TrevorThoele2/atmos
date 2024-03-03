#pragma once

#include "JavaScriptType.h"
#include "ImageAssetGridSize.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Asset::ImageGridSize>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Asset::ImageGridSize& value);
        [[nodiscard]] static std::optional<Asset::ImageGridSize> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Asset::ImageGridSize::Dimension> columns;
            Property<Asset::ImageGridSize::Dimension> rows;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}