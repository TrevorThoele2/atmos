
#include "AngelScriptUtility.h"

#include <angelscript.h>

namespace Atmos::Scripting
{
    struct VariantVisitImplementation
    {
        static void Do(bool value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgByte(index, value);
        }

        static void Do(std::int8_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgByte(index, value);
        }

        static void Do(std::int16_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgWord(index, value);
        }

        static void Do(std::int32_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgDWord(index, value);
        }

        static void Do(std::int64_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgQWord(index, value);
        }

        static void Do(std::uint8_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgByte(index, value);
        }

        static void Do(std::uint16_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgWord(index, value);
        }

        static void Do(std::uint32_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgDWord(index, value);
        }

        static void Do(std::uint64_t value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgQWord(index, value);
        }

        static void Do(float value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgFloat(index, value);
        }

        static void Do(double value, ParameterIndex index, asIScriptContext* context)
        {
            context->SetArgDouble(index, value);
        }
    };

    void PushVariantToAngelScriptParameter(ParameterIndex index, VariantT& variant, asIScriptContext* context)
    {
        ::Chroma::Visit<VariantVisitImplementation>(variant, index, context);
    }
}