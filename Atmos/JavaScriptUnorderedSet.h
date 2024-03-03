#pragma once

#include <v8.h>

#include "JavaScriptType.h"
#include <unordered_set>

#include "ScriptError.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    class Type<std::unordered_set<T>>
    {
    public:
        using V8T = v8::Array;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const std::unordered_set<T>& value);
        [[nodiscard]] static std::optional<std::unordered_set<T>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<class T>
    [[nodiscard]] auto Type<std::unordered_set<T>>::ToV8(v8::Isolate& isolate, const std::unordered_set<T>& value) -> v8::Local<V8T>
    {
        const auto context = isolate.GetCurrentContext();

        const auto javaScriptArray = v8::Array::New(&isolate, value.size());
        size_t i = 0;
        for (auto& element : value)
        {
            javaScriptArray->Set(context, i, JavaScript::ToV8(isolate, element));
            ++i;
        }

        return javaScriptArray;
    }

    template<class T>
    std::optional<std::unordered_set<T>> Type<std::unordered_set<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        if (!value->IsArray())
            return std::optional<std::unordered_set<T>>{};

        auto& array = *v8::Array::Cast(*value);

        std::unordered_set<T> unordered_set;
        for (size_t i = 0; i < array.Length(); ++i)
        {
            const auto element = JavaScript::FromV8<T>(isolate, array.Get(isolate.GetCurrentContext(), i));
            if (!element)
                return std::optional<std::unordered_set<T>>{};
            unordered_set.emplace(*element);
        }
        return unordered_set;
    }
}