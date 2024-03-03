
#include "ActionParameter.h"

#include <Chroma/Iterate.h>

namespace Atmos
{
    namespace Act
    {
        ParameterType GetParameterType(size_t from)
        {
            return static_cast<ParameterType>(from);
        }

        ParameterType GetParameterType(const Parameter &from)
        {
            if (!from.IsInhabited())
                return ParameterType::NONE;

            return GetParameterType(from.GetTypeAsID());
        }

        bool IsListParameter(ParameterType type)
        {
            return type == ParameterType::VECTOR_BOOL || type == ParameterType::VECTOR_UINT_8 || type == ParameterType::VECTOR_UINT_16 ||
                type == ParameterType::VECTOR_UINT_32 || type == ParameterType::VECTOR_UINT_64 || type == ParameterType::VECTOR_INT_8 ||
                type == ParameterType::VECTOR_INT_16 || type == ParameterType::VECTOR_INT_32 || type == ParameterType::VECTOR_INT_64 ||
                type == ParameterType::VECTOR_FLOAT || type == ParameterType::VECTOR_DOUBLE || type == ParameterType::VECTOR_FIXED_64 ||
                type == ParameterType::VECTOR_STRING || type == ParameterType::VECTOR_GRID_POSITION || type == ParameterType::VECTOR_NAME_VALUE_PAIR ||
                type == ParameterType::VECTOR_SINGLE_VALUE;
        }

        bool IsListParameter(const Parameter &parameter)
        {
            return IsListParameter(GetParameterType(parameter));
        }

        ParameterType GetSingularTypeOfList(ParameterType type)
        {
            if (type == ParameterType::VECTOR_BOOL)
                return ParameterType::BOOL;

            if (type == ParameterType::VECTOR_UINT_8)
                return ParameterType::UINT_8;
            
            if (type == ParameterType::VECTOR_UINT_16)
                return ParameterType::UINT_16;

            if (type == ParameterType::VECTOR_UINT_32)
                return ParameterType::UINT_32;
            
            if (type == ParameterType::VECTOR_UINT_64)
                return ParameterType::UINT_64;
            
            if (type == ParameterType::VECTOR_INT_8)
                return ParameterType::INT_8;

            if (type == ParameterType::VECTOR_INT_16)
                return ParameterType::INT_16;
            
            if (type == ParameterType::VECTOR_INT_32)
                return ParameterType::INT_32;
            
            if (type == ParameterType::VECTOR_INT_64)
                return ParameterType::INT_64;

            if (type == ParameterType::VECTOR_FLOAT)
                return ParameterType::FLOAT;
            
            if (type == ParameterType::VECTOR_DOUBLE)
                return ParameterType::DOUBLE;
            
            if (type == ParameterType::VECTOR_FIXED_64)
                return ParameterType::FIXED_64;

            if (type == ParameterType::VECTOR_STRING)
                return ParameterType::STRING;
            
            if (type == ParameterType::VECTOR_GRID_POSITION)
                return ParameterType::GRID_POSITION;
            
            if (type == ParameterType::VECTOR_NAME_VALUE_PAIR)
                return ParameterType::NAME_VALUE_PAIR;

            return ParameterType::NONE;
        }

        ParameterType GetSingularTypeOfList(const Parameter &parameter)
        {
            return GetSingularTypeOfList(GetParameterType(parameter.GetTypeAsID()));
        }
    }
}

namespace Chroma
{
    template Variant<
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
        ParameterTypeError::ParameterTypeError() : runtime_error("A parameter has been attempted to be extracted as a different type than it is.")
        {}

        Parameter::Parameter()
        {}

        Parameter::Parameter(const Variant &variant) : val(variant)
        {}

        Parameter::Parameter(Variant &&variant) : val(std::move(variant))
        {}

        Parameter::Parameter(const Parameter &arg) : val(arg.val)
        {}

        Parameter::Parameter(Parameter &&arg) : val(std::move(arg.val))
        {}

        Parameter& Parameter::operator=(const Variant &arg)
        {
            val = arg;
            return *this;
        }

        Parameter& Parameter::operator=(Variant &&arg)
        {
            val = std::move(arg);
            return *this;
        }

        Parameter& Parameter::operator=(const Parameter &arg)
        {
            val = arg.val;
            return *this;
        }

        Parameter& Parameter::operator=(Parameter &&arg)
        {
            val = std::move(arg.val);
            return *this;
        }

        Variant* Parameter::operator->()
        {
            return &val;
        }

        const Variant* Parameter::operator->() const
        {
            return &val;
        }

        Variant& Parameter::operator*()
        {
            return val;
        }

        const Variant& Parameter::operator*() const
        {
            return val;
        }

        Variant& Parameter::Get()
        {
            return val;
        }

        const Variant& Parameter::Get() const
        {
            return val;
        }

        Variant::ID Parameter::GetTypeAsID() const
        {
            return val.GetTypeAsID();
        }
        */
    }
}