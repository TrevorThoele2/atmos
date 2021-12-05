#pragma once

#include "JavaScriptType.h"
#include "FindEntityByPosition.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Atmos::Entity::FindByPosition>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Atmos::Entity::FindByPosition& value);
        [[nodiscard]] static std::optional<Atmos::Entity::FindByPosition> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Spatial::Grid::Point> position;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}