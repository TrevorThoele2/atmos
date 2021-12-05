#pragma once

#include <v8.h>

#include "JavaScriptType.h"
#include <set>

#include "ScriptError.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    class Type<std::set<T>>
    {
    public:
        using V8T = v8::Array;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const std::set<T>& value);
        [[nodiscard]] static std::optional<std::set<T>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<class T>
    [[nodiscard]] auto Type<std::set<T>>::ToV8(v8::Isolate& isolate, const std::set<T>& value) -> v8::Local<V8T>
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
    std::optional<std::set<T>> Type<std::set<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        if (!value->IsArray())
            return std::optional<std::set<T>>{};

        auto& array = *v8::Array::Cast(*value);

        std::set<T> set;
        for (size_t i = 0; i < array.Length(); ++i)
        {
            const auto element = JavaScript::FromV8<T>(isolate, array.Get(isolate.GetCurrentContext(), i));
            if (!element)
                return std::optional<std::set<T>>{};
            set.emplace(*element);
        }
        return set;
    }
}