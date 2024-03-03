#include "JavaScriptNumeric.h"

namespace Atmos::Scripting::JavaScript
{
    template<class T>
    std::optional<T> FromV8Numeric(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        const auto context = isolate.GetCurrentContext();
        return value.IsEmpty()
			? std::optional<T>{}
			: value->IsBigInt()
            ? static_cast<T>(value->ToBigInt(context).ToLocalChecked()->Int64Value())
            : value->IsInt32()
			? static_cast<T>(value->ToInt32(context).ToLocalChecked()->Value())
			: value->IsNumber()
			? static_cast<T>(value->ToNumber(context).ToLocalChecked()->Value())
            : std::optional<T>{};
    }

    auto Type<std::int8_t>::ToV8(v8::Isolate& isolate, const std::int8_t& value) -> v8::Local<V8T>
    {
        return v8::Int32::New(&isolate, value);
    }

    std::optional<std::int8_t> Type<std::int8_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::int8_t>(isolate, value);
    }

    auto Type<std::int16_t>::ToV8(v8::Isolate& isolate, const std::int16_t& value) -> v8::Local<V8T>
    {
        return v8::Int32::New(&isolate, value);
    }

    std::optional<std::int16_t> Type<std::int16_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::int16_t>(isolate, value);
    }

    auto Type<std::int32_t>::ToV8(v8::Isolate& isolate, const std::int32_t& value) -> v8::Local<V8T>
    {
        return v8::Int32::New(&isolate, value);
    }

    std::optional<std::int32_t> Type<std::int32_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::int32_t>(isolate, value);
    }

    auto Type<std::int64_t>::ToV8(v8::Isolate& isolate, const std::int64_t& value) -> v8::Local<V8T>
    {
        return v8::BigInt::New(&isolate, value);
    }

    std::optional<std::int64_t> Type<std::int64_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::int64_t>(isolate, value);
    }

    auto Type<std::uint8_t>::ToV8(v8::Isolate& isolate, const std::uint8_t& value) -> v8::Local<V8T>
    {
        return v8::Uint32::New(&isolate, value);
    }

    std::optional<std::uint8_t> Type<std::uint8_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::uint8_t>(isolate, value);
    }

    auto Type<std::uint16_t>::ToV8(v8::Isolate& isolate, const std::uint16_t& value) -> v8::Local<V8T>
    {
        return v8::Uint32::New(&isolate, value);
    }

    std::optional<std::uint16_t> Type<std::uint16_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::uint16_t>(isolate, value);
    }

    auto Type<std::uint32_t>::ToV8(v8::Isolate& isolate, const std::uint32_t& value) -> v8::Local<V8T>
    {
        return v8::BigInt::New(&isolate, value);
    }

    std::optional<std::uint32_t> Type<std::uint32_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::uint32_t>(isolate, value);
    }

    auto Type<std::uint64_t>::ToV8(v8::Isolate& isolate, const std::uint64_t& value) -> v8::Local<V8T>
    {
        return v8::BigInt::New(&isolate, static_cast<std::int64_t>(value));
    }

    std::optional<std::uint64_t> Type<std::uint64_t>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<std::uint64_t>(isolate, value);
    }

    auto Type<float>::ToV8(v8::Isolate& isolate, const float& value) -> v8::Local<V8T>
    {
        return v8::Number::New(&isolate, static_cast<double>(value));
    }

    std::optional<float> Type<float>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<float>(isolate, value);
    }

    auto Type<double>::ToV8(v8::Isolate& isolate, const double& value) -> v8::Local<V8T>
    {
        return v8::Number::New(&isolate, value);
    }

    std::optional<double> Type<double>::FromV8(v8::Isolate& isolate, const v8::Local<v8::Value>& value)
    {
        return FromV8Numeric<double>(isolate, value);
    }
}