#pragma once

#include "JavaScriptType.h"

namespace Atmos::Scripting::JavaScript
{
    template<>
    class Type<std::int8_t>
    {
    public:
        using V8T = v8::Integer;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const int8_t& value);
        [[nodiscard]] static std::optional<int8_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::int16_t>
    {
    public:
        using V8T = v8::Integer;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const int16_t& value);
        [[nodiscard]] static std::optional<int16_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::int32_t>
    {
    public:
        using V8T = v8::Integer;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const int32_t& value);
        [[nodiscard]] static std::optional<int32_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::int64_t>
    {
    public:
        using V8T = v8::BigInt;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const int64_t& value);
        [[nodiscard]] static std::optional<int64_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::uint8_t>
    {
    public:
        using V8T = v8::Integer;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const uint8_t& value);
        [[nodiscard]] static std::optional<uint8_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::uint16_t>
    {
    public:
        using V8T = v8::Integer;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const uint16_t& value);
        [[nodiscard]] static std::optional<uint16_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::uint32_t>
    {
    public:
        using V8T = v8::BigInt;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const uint32_t& value);
        [[nodiscard]] static std::optional<uint32_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<std::uint64_t>
    {
    public:
        using V8T = v8::BigInt;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const uint64_t& value);
        [[nodiscard]] static std::optional<uint64_t> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<float>
    {
    public:
        using V8T = v8::Number;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const float& value);
        [[nodiscard]] static std::optional<float> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };

    template<>
    class Type<double>
    {
    public:
        using V8T = v8::Number;

        [[nodiscard]] static v8::Local<V8T> ToV8(v8::Isolate& isolate, const double& value);
        [[nodiscard]] static std::optional<double> FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value);
    };
}