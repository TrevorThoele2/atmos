#pragma once

#include "JavaScriptType.h"
#include "MoveEntityTo.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Atmos::Entity::MoveTo>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Atmos::Entity::MoveTo& value);
        [[nodiscard]] static std::optional<Atmos::Entity::MoveTo> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> entity;
            Property<Spatial::Grid::Point> to;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}