#pragma once

#include "JavaScriptType.h"
#include "ChangeTextCore.h"

#include "JavaScriptFontAsset.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::ChangeTextCore>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::ChangeTextCore& value);
        [[nodiscard]] static std::optional<Render::ChangeTextCore> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<String>> string;
            Property<std::optional<FontAsset>> font;
            Property<std::optional<float>> wrapWidth;
            Property<std::optional<bool>> bold;
            Property<std::optional<bool>> italics;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}