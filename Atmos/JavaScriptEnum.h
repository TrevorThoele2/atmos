#pragma once

#include "JavaScriptType.h"
#include "JavaScriptNumeric.h"
#include "JavaScriptObject.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    class EnumType
    {
    public:
        using V8T = v8::Number;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const T& value);
        [[nodiscard]] static std::optional<T> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<class T>
	auto EnumType<T>::ToV8(v8::Isolate& isolate, const T& value) -> v8::Local<V8T>
    {
        return JavaScript::ToV8(isolate, static_cast<std::underlying_type_t<T>>(value));
    }

    template<class T>
	std::optional<T> EnumType<T>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto from = JavaScript::FromV8<std::underlying_type_t<T>>(isolate, value);
        return from ? static_cast<T>(*from) : std::optional<T>{};
    }

    template<class T>
    struct EnumProperty
    {
        String name;
        T value;
    };

    template<class T>
    [[nodiscard]] v8::Local<v8::Object> CreateEnum(v8::Isolate& isolate, const std::vector<EnumProperty<T>>& properties)
    {
        std::vector<AnyProperty> useProperties;
        useProperties.reserve(properties.size());
        for (auto& property : properties)
            useProperties.emplace_back(property.name, ToV8(isolate, static_cast<std::underlying_type_t<T>>(property.value)));

        return CreateObject(isolate, useProperties);
    }
}