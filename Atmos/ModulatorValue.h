#pragma once

#include <cstdint>

#include "Serialization.h"

#include <Chroma\Variant.h>

namespace Atmos
{
    namespace Modulator
    {
        class Value
        {
        public:
            enum class Type : unsigned char
            {
                NONE,
                INT,
                FLOAT
            };
        private:
            typedef ::Chroma::Variant<std::int64_t, float> VariantT;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            VariantT variant;
        public:
            explicit Value();
            explicit Value(std::int64_t i);
            explicit Value(float f);
            Value(const Value &arg) = default;
            Value& operator=(const Value &arg) = default;
            ~Value();
            bool operator==(const Value &arg) const;
            bool operator!=(const Value &arg) const;

            void Set();
            void Set(std::int64_t set);
            void Set(float set);
            void Convert(Type type);

            bool IsNone() const;
            bool IsInt() const;
            bool IsFloat() const;
            Type GetType() const;

            std::int64_t AsInt() const;
            float AsFloat() const;
        };

        template<Value::Type>
        struct ValueTraits;

        template<>
        struct ValueTraits<Value::Type::INT>
        {
            typedef std::int64_t T;
        };

        template<>
        struct ValueTraits<Value::Type::FLOAT>
        {
            typedef float T;
        };
    }
}