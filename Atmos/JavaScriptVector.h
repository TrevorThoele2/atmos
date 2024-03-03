#pragma once

#include <v8.h>

#include "JavaScriptType.h"
#include <vector>

#include "ScriptError.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    class Type<std::vector<T>>
    {
    public:
        using V8T = v8::Array;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const std::vector<T>& value);
        [[nodiscard]] static std::optional<std::vector<T>> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<class T>
    [[nodiscard]] auto Type<std::vector<T>>::ToV8(v8::Isolate& isolate, const std::vector<T>& value) -> v8::Local<V8T>
    {
        std::vector<v8::Local<v8::Value>> vector;
        vector.reserve(value.size());
        for (auto& element : value)
            vector.push_back(JavaScript::ToV8(isolate, element));
        return v8::Array::New(&isolate, vector.data(), vector.size());
    }

    template<class T>
    std::optional<std::vector<T>> Type<std::vector<T>>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        if (!value->IsArray())
            return std::optional<std::vector<T>>{};

        auto& array = *v8::Array::Cast(*value);
        
        std::vector<T> vector;
        vector.reserve(array.Length());
        for (size_t i = 0; i < array.Length(); ++i)
        {
            const auto element = JavaScript::FromV8<T>(isolate, array.Get(isolate.GetCurrentContext(), i));
            if (!element)
                return std::optional<std::vector<T>>{};
			vector.push_back(*element);
        }
        return vector;
    }
}