#include "AngelScriptVariant.h"

#include "AngelScriptObjectRegistration.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Variant>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, bool>>, { "bool value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::int8_t>>, { "int8 value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::int16_t>>, { "int16 value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::int32_t>>, { "int value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::int64_t>>, { "int64 value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::uint8_t>>, { "uint8 value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::uint16_t>>, { "uint16 value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::uint32_t>>, { "uint value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, std::uint64_t>>, { "uint64 value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, float>>, { "float value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, double>>, { "double value" })
            .Constructor(&Management::GenerateValue<&PullFromParameter<0, String>>, { "string value" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Assignment(&Management::Assign<bool>, "const bool &in")
            .Assignment(&Management::Assign<std::int8_t>, "const int8 &in")
            .Assignment(&Management::Assign<std::int16_t>, "const int16 &in")
            .Assignment(&Management::Assign<std::int32_t>, "const int &in")
            .Assignment(&Management::Assign<std::int64_t>, "const int64 &in")
            .Assignment(&Management::Assign<std::uint8_t>, "const uint8 &in")
            .Assignment(&Management::Assign<std::uint16_t>, "const uint16 &in")
            .Assignment(&Management::Assign<std::uint32_t>, "const uint &in")
            .Assignment(&Management::Assign<std::uint64_t>, "const uint64 &in")
            .Assignment(&Management::Assign<float>, "const float &in")
            .Assignment(&Management::Assign<double>, "const double &in")
            .Assignment(&Management::Assign<String>, "const string &in")
            .ConstMethod(&Management::Method<&AsBool>, "bool", "AsBool", {})
            .ConstMethod(&Management::Method<&AsInt>, "int64", "AsInt", {})
            .ConstMethod(&Management::Method<AsFloat>, "double", "AsFloat", {})
            .ConstMethod(&Management::Method<&AsString>, "string", "AsString", {})
            .ConstMethod(&Management::Method<&IsBool>, "bool", "IsBool", {})
            .ConstMethod(&Management::Method<&IsInt>, "bool", "IsInt", {})
            .ConstMethod(&Management::Method<&IsFloat>, "bool", "IsFloat", {})
            .ConstMethod(&Management::Method<&IsString>, "bool", "IsString", {})
            .Actualize(engine, documentationManager);
    }

    bool Registration<Variant>::AsBool(Type type)
    {
        return std::get<bool>(type);
    }

    std::int64_t Registration<Variant>::AsInt(Type type)
    {
        if (std::holds_alternative<std::int8_t>(type))
            return std::get<std::int8_t>(type);
        else if (std::holds_alternative<std::int16_t>(type))
            return std::get<std::int16_t>(type);
        else if (std::holds_alternative<std::int32_t>(type))
            return std::get<std::int32_t>(type);
        else if (std::holds_alternative<std::int64_t>(type))
            return std::get<std::int64_t>(type);
        else if (std::holds_alternative<std::uint8_t>(type))
            return std::get<std::uint8_t>(type);
        else if (std::holds_alternative<std::uint16_t>(type))
            return std::get<std::uint16_t>(type);
        else if (std::holds_alternative<std::uint32_t>(type))
            return std::get<std::uint32_t>(type);
        else
            return std::get<std::uint64_t>(type);
    }

    double Registration<Variant>::AsFloat(Type type)
    {
        if (std::holds_alternative<float>(type))
            return std::get<float>(type);
        else
            return std::get<double>(type);
    }

    String Registration<Variant>::AsString(Type type)
    {
        return std::get<String>(type);
    }

    bool Registration<Variant>::IsBool(Type type)
    {
        return std::holds_alternative<bool>(type);
    }

    bool Registration<Variant>::IsInt(Type type)
    {
        return
            std::holds_alternative<std::int8_t>(type) ||
            std::holds_alternative<std::int16_t>(type) ||
            std::holds_alternative<std::int32_t>(type) ||
            std::holds_alternative<std::int64_t>(type) ||
            std::holds_alternative<std::uint8_t>(type) ||
            std::holds_alternative<std::uint16_t>(type) ||
            std::holds_alternative<std::uint32_t>(type) ||
            std::holds_alternative<std::uint64_t>(type);
    }

    bool Registration<Variant>::IsFloat(Type type)
    {
        return std::holds_alternative<float>(type) || std::holds_alternative<double>(type);
    }

    bool Registration<Variant>::IsString(Type type)
    {
        return std::holds_alternative<String>(type);
    }
}