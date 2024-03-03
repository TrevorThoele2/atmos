#pragma once

#include "JavaScriptType.h"
#include "MoveGridRegion.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::MoveGridRegion>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::MoveGridRegion& value);
        [[nodiscard]] static std::optional<Render::MoveGridRegion> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<std::unordered_set<Spatial::Grid::Point>>> points;
            Property<std::optional<Spatial::Grid::Point::Value>> z;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}