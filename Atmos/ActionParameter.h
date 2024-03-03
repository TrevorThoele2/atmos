#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "ActionID.h"
#include "Variant.h"
#include "NameValuePair.h"

namespace Atmos
{
    namespace Act
    {
        typedef Variant::VariadicTemplateT::Append<NameValuePair>::Type::ForwardArguments<::Chroma::Variant>::Type ParameterValueSingle;

        typedef ::Chroma::Variant<
            bool,
            std::int8_t,
            std::int16_t,
            std::int32_t,
            std::int64_t,
            std::uint8_t,
            std::uint16_t,
            std::uint32_t,
            std::uint64_t,
            float,
            double,
            FixedPoint64,
            String,
            GridPosition,
            NameValuePair,
            std::vector<bool>,
            std::vector<std::int8_t>,
            std::vector<std::int16_t>,
            std::vector<std::int32_t>,
            std::vector<std::int64_t>,
            std::vector<std::uint8_t>,
            std::vector<std::uint16_t>,
            std::vector<std::uint32_t>,
            std::vector<std::uint64_t>,
            std::vector<float>,
            std::vector<double>,
            std::vector<FixedPoint64>,
            std::vector<String>,
            std::vector<GridPosition>,
            std::vector<NameValuePair>,
            std::vector<ParameterValueSingle >> Parameter;

        enum class ParameterType : std::int32_t
        {
            BOOL,
            INT_8,
            INT_16,
            INT_32,
            INT_64,
            UINT_8,
            UINT_16,
            UINT_32,
            UINT_64,
            FLOAT,
            DOUBLE,
            FIXED_64,
            STRING,
            GRID_POSITION,
            NAME_VALUE_PAIR,

            VECTOR_BOOL,
            VECTOR_UINT_8,
            VECTOR_UINT_16,
            VECTOR_UINT_32,
            VECTOR_UINT_64,
            VECTOR_INT_8,
            VECTOR_INT_16,
            VECTOR_INT_32,
            VECTOR_INT_64,
            VECTOR_FLOAT,
            VECTOR_DOUBLE,
            VECTOR_FIXED_64,
            VECTOR_STRING,
            VECTOR_GRID_POSITION,
            VECTOR_NAME_VALUE_PAIR,
            VECTOR_SINGLE_VALUE,

            NONE = Parameter::uninhabitedID
        };

        typedef ::Chroma::EnumIterationTraits<ParameterType, ParameterType::BOOL, ParameterType::VECTOR_SINGLE_VALUE> ParameterIterationTraits;

        ParameterType GetParameterType(size_t from);
        ParameterType GetParameterType(const Parameter &from);
        bool IsListParameter(ParameterType type);
        bool IsListParameter(const Parameter &parameter);
        // Returns NONE if the type is not a list
        ParameterType GetSingularTypeOfList(ParameterType type);
        // Returns NONE if the type is not a list
        ParameterType GetSingularTypeOfList(const Parameter &parameter);
    }
}

namespace Chroma
{
    extern template Variant<
        bool,
        std::int8_t,
        std::int16_t,
        std::int32_t,
        std::int64_t,
        std::uint8_t,
        std::uint16_t,
        std::uint32_t,
        std::uint64_t,
        float,
        double,
        ::Atmos::FixedPoint64,
        ::Atmos::String,
        ::Atmos::GridPosition,
        ::Atmos::NameValuePair,
        std::vector<bool>,
        std::vector<std::int8_t>,
        std::vector<std::int16_t>,
        std::vector<std::int32_t>,
        std::vector<std::int64_t>,
        std::vector<std::uint8_t>,
        std::vector<std::uint16_t>,
        std::vector<std::uint32_t>,
        std::vector<std::uint64_t>,
        std::vector<float>,
        std::vector<double>,
        std::vector<::Atmos::FixedPoint64>,
        std::vector<::Atmos::String>,
        std::vector<::Atmos::GridPosition>,
        std::vector<::Atmos::NameValuePair>,
        std::vector<::Atmos::Act::ParameterValueSingle>>;
}

namespace Atmos
{
    namespace Act
    {

        /*
        class ParameterTypeError : public std::runtime_error
        {
        public:
            ParameterTypeError();
        };

        class Parameter
        {
        private:
            Variant val;
        public:
            Parameter();
            Parameter(const Variant &variant);
            Parameter(Variant &&variant);
            Parameter(const Parameter &arg);
            Parameter(Parameter &&arg);
            Parameter& operator=(const Variant &arg);
            Parameter& operator=(Variant &&arg);
            Parameter& operator=(const Parameter &arg);
            Parameter& operator=(Parameter &&arg);

            Variant* operator->();
            const Variant* operator->() const;
            Variant& operator*();
            const Variant& operator*() const;
            Variant& Get();
            const Variant& Get() const;
            template<class T>
            T& GetAs();
            template<class T>
            const T& GetAs() const;

            Variant::ID GetTypeAsID() const;
        };

        template<class T>
        T& Parameter::GetAs()
        {
            if (!val.Is<T>())
                throw ParameterTypeError();

            return val.Get<T>();
        }

        template<class T>
        const T& Parameter::GetAs() const
        {
            if (!val.Is<T>())
                throw ParameterTypeError();

            return val.Get<T>();
        }
        */
    }
}