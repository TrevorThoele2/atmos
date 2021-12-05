#pragma once

#include "JavaScriptType.h"
#include "ChangeImageCore.h"

#include "JavaScriptImageAsset.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::ChangeImageCore>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::ChangeImageCore& value);
        [[nodiscard]] static std::optional<Render::ChangeImageCore> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<ImageAsset>> asset;
            Property<std::optional<Render::ImageCore::Index>> assetIndex;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}