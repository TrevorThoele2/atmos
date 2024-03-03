#pragma once

#include "JavaScriptType.h"
#include "MoveLine.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::MoveLine>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::MoveLine& value);
        [[nodiscard]] static std::optional<Render::MoveLine> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<std::vector<Spatial::Point2D>>> points;
            Property<std::optional<Spatial::Point2D::Value>> z;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}