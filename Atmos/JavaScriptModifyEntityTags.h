#pragma once

#include "JavaScriptType.h"
#include "ModifyEntityTags.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Atmos::Entity::ModifyTags>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Atmos::Entity::ModifyTags& value);
        [[nodiscard]] static std::optional<Atmos::Entity::ModifyTags> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> entity;
            Property<std::vector<Atmos::Tag>> add;
            Property<std::vector<Atmos::Tag>> remove;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}