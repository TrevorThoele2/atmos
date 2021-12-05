#pragma once

#include "JavaScriptType.h"
#include "CanMoveEntityTo.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Atmos::Entity::CanMoveTo>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Atmos::Entity::CanMoveTo& value);
        [[nodiscard]] static std::optional<Atmos::Entity::CanMoveTo> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
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