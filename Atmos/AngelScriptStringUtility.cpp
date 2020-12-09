#include "AngelScriptStringUtility.h"

#include <angelscript.h>
#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<StringUtility>::RegisterTo(asIScriptEngine& engine)
    {
        GlobalRegistration(containingNamespace)
            .Function(&Management::Function<&Type::ToString<bool>>,
                "string", "ToString", { "bool value" })
            .Function(&Management::Function<&Type::ToString<std::int8_t>>,
                "string", "ToString", { "int8 value" })
            .Function(&Management::Function<&Type::ToString<std::int16_t>>,
                "string", "ToString", { "int16 value" })
            .Function(&Management::Function<&Type::ToString<std::int32_t>>,
                "string", "ToString", { "int value" })
            .Function(&Management::Function<&Type::ToString<std::int64_t>>,
                "string", "ToString", { "int64 value" })
            .Function(&Management::Function<&Type::ToString<std::uint8_t>>,
                "string", "ToString", { "uint8 value" })
            .Function(&Management::Function<&Type::ToString<std::uint16_t>>,
                "string", "ToString", { "uint16 value" })
            .Function(&Management::Function<&Type::ToString<std::uint32_t>>,
                "string", "ToString", { "uint value" })
            .Function(&Management::Function<&Type::ToString<std::uint64_t>>,
                "string", "ToString", { "uint64 value" })
            .Function(&Management::Function<&Type::ToString<float>>,
                "string", "ToString", { "float value" })
            .Function(&Management::Function<&Type::ToString<double>>,
                "string", "ToString", { "double value" })
            .Function(&Management::Function<Type::ToString<std::string>>,
                "string", "ToString", { "string value" })

            .Function(&Management::Function<&Type::FromString<bool>>,
                "bool", "ToBool", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::int8_t>>,
                "int8", "ToInt8", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::int16_t>>,
                "int16", "ToInt16", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::int32_t>>,
                "int", "ToInt", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::int64_t>>,
                "int64", "ToInt64", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::uint8_t>>,
                "uint8", "ToUint8", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::uint16_t>>,
                "uint16", "ToUint16", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::uint32_t>>,
                "uint", "ToUint", { "string value" })
            .Function(&Management::Function<&Type::FromString<std::uint64_t>>,
                "uint64", "ToUint64", { "string value" })
            .Function(&Management::Function<&Type::FromString<float>>,
                "float", "ToFloat", { "string value" })
            .Function(&Management::Function<&Type::FromString<double>>,
                "double", "ToDouble", { "string value" })

            .Function(&Management::Function<&Join>, "string", "Join", { "string joiner", "string[]@ tokens" })

            .Actualize(engine);
    }

    String Registration<StringUtility>::Join(String joiner, std::vector<String> tokens)
    {
        return Chroma::Join(joiner, tokens.begin(), tokens.end());
    }
}