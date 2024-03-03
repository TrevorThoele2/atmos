#include "AngelScriptMath.h"

#include "MathUtility.h"

#include <angelscript.h>
#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Math>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        GlobalRegistration(ContainingNamespace())
            .Function(&Management::Function<&Floor<float>>,
                "float", "Floor", { "float value" })
            .Function(&Management::Function<&Floor<double>>,
                "double", "Floor", { "double value" })
            .Function(&Management::Function<&Ceiling<float>>,
                "float", "Ceiling", { "float value" })
            .Function(&Management::Function<&Floor<double>>,
                "double", "Ceiling", { "double value" })
            .Function(&Management::Function<&Round<float>>,
                "float", "Round", { "float value" })
            .Function(&Management::Function<&Round<double>>,
                "double", "Round", { "double value" })
            .Function(&Management::Function<&AbsoluteValue<std::int8_t, long long>>,
                "int8", "Abs", { "int8 value" })
            .Function(&Management::Function<&AbsoluteValue<std::int16_t, long long>>,
                "int16", "Abs", { "int16 value" })
            .Function(&Management::Function<&AbsoluteValue<std::int32_t, long long>>,
                "int", "Abs", { "int value" })
            .Function(&Management::Function<&AbsoluteValue<std::int64_t, long long>>,
                "int64", "Abs", { "int64 value" })
            .Function(&Management::Function<&AbsoluteValue<std::uint8_t, long long>>,
                "uint8", "Abs", { "uint8 value" })
            .Function(&Management::Function<&AbsoluteValue<std::uint16_t, long long>>,
                "uint16", "Abs", { "uint16 value" })
            .Function(&Management::Function<&AbsoluteValue<std::uint32_t, long long>>,
                "uint", "Abs", { "uint value" })
            .Function(&Management::Function<&AbsoluteValue<std::uint64_t, long long>>,
                "uint64", "Abs", { "uint64 value" })
            .Function(&Management::Function<&AbsoluteValue<float, double>>,
                "float", "Abs", { "float value" })
            .Function(&Management::Function<&AbsoluteValue<double, double>>,
                "double", "Abs", { "double value" })
            .Function(&Management::Function<&Sin<float>>,
                "float", "Sin", { "float value" })
            .Function(&Management::Function<&Sin<double>>,
                "double", "Sin", { "double value" })
            .Function(&Management::Function<&Cos<float>>,
                "float", "Cos", { "float value" })
            .Function(&Management::Function<&Cos<double>>,
                "double", "Cos", { "double value" })
            .Function(&Management::Function<&Tan<float>>,
                "float", "Tan", { "float value" })
            .Function(&Management::Function<&Tan<double>>,
                "double", "Tan", { "double value" })
            .Function(&Management::Function<&Asin<float>>,
                "float", "Asin", { "float value" })
            .Function(&Management::Function<&Asin<double>>,
                "double", "Asin", { "double value" })
            .Function(&Management::Function<&Acos<float>>,
                "float", "Acos", { "float value" })
            .Function(&Management::Function<&Acos<double>>,
                "double", "Acos", { "double value" })
            .Function(&Management::Function<&Atan<float>>,
                "float", "Atan", { "float value" })
            .Function(&Management::Function<&Atan<double>>,
                "double", "Atan", { "double value" })
            .Function(&Management::Function<&Pi>,
                "double", "Pi", {})
            .Actualize(engine, documentationManager);
    }

    double Registration<Math>::Pi()
    {
        return pi<double>;
    }
}