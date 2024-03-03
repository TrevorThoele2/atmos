#pragma once

#include "JavaScriptType.h"
#include "FindImagesByBox.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::FindImagesByBox>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::FindImagesByBox& value);
        [[nodiscard]] static std::optional<Render::FindImagesByBox> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::AxisAlignedBox3D> box;
            Property<Spatial::Space> space;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}