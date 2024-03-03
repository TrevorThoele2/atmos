#pragma once

#include "JavaScriptType.h"
#include "FindAssetByName.h"

#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    class Type<Atmos::Asset::FindByName<T>>
    {
    public:
        using V8T = v8::Object;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const Atmos::Asset::FindByName<T>& value);
        [[nodiscard]] static std::optional<Atmos::Asset::FindByName<T>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    private:
        struct Definition
        {
            Property<String> name;

            Definition();

            [[nodiscard]] AnyObjectDefinition ToAny();
        };
    };

    template<class T>
    auto Type<Atmos::Asset::FindByName<T>>::ToV8(v8::Isolate& isolate, const Atmos::Asset::FindByName<T>& value) -> v8::Local<V8T>
    {
        Definition definition;
        definition.name.SetValue(isolate, value.name);
        return CreateObject(isolate, definition.ToAny());
    }

    template<class T>
    std::optional<Atmos::Asset::FindByName<T>> Type<Atmos::Asset::FindByName<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromObject<Atmos::Asset::FindByName<T>, Definition>(
            isolate,
            value,
            [](const Definition& definition)
            {
                const auto name = definition.name.AtmosValue();
                return name
                    ? Atmos::Asset::FindByName<T>
                    {
                        *name
                    }
                    : std::optional<Atmos::Asset::FindByName<T>>{};
            });
    }

    template<class T>
    Type<Atmos::Asset::FindByName<T>>::Definition::Definition() :
        name("name")
    {}

    template<class T>
    AnyObjectDefinition Type<Atmos::Asset::FindByName<T>>::Definition::ToAny()
    {
        return
        {
            {
                { name }
            }
        };
    }
}