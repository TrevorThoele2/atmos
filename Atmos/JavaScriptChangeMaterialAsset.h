#pragma once

#include "JavaScriptType.h"
#include "ChangeMaterialAsset.h"

#include "JavaScriptMaterialAsset.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<Render::ChangeMaterialAsset>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Render::ChangeMaterialAsset& value);
        [[nodiscard]] static std::optional<Render::ChangeMaterialAsset> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<Arca::RelicID> id;
            Property<std::optional<MaterialAsset>> to;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };
}