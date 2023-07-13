#pragma once

#include "JavaScriptType.h"
#include "Shaders.h"

#include "JavaScriptShaderAsset.h"
#include "JavaScriptOptional.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::Shaders>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::Shaders& value);
        [[nodiscard]] static std::optional<Render::Shaders> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<std::optional<ShaderAsset>> vertex;
            Property<std::optional<ShaderAsset>> fragment;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}