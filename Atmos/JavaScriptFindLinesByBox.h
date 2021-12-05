#pragma once

#include "JavaScriptType.h"
#include "FindLinesByBox.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::FindLinesByBox>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::FindLinesByBox& value);
        [[nodiscard]] static std::optional<Render::FindLinesByBox> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::AxisAlignedBox3D> box;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}