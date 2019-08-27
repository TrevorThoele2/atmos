#include "AngelScriptUtility.h"

#include <angelscript.h>

namespace Atmos::Script::Angel
{
    struct VariantVisitImplementation
    {
        ParameterIndex index;
        asIScriptContext* context;

        VariantVisitImplementation(ParameterIndex index, asIScriptContext* context) : index(index), context(context)
        {}

        void operator()(bool value) const
        {
            context->SetArgByte(index, value);
        }

        void operator()(std::int8_t value) const
        {
            context->SetArgByte(index, value);
        }

        void operator()(std::int16_t value) const
        {
            context->SetArgWord(index, value);
        }

        void operator()(std::int32_t value) const
        {
            context->SetArgDWord(index, value);
        }

        void operator()(std::int64_t value) const
        {
            context->SetArgQWord(index, value);
        }

        void operator()(std::uint8_t value) const
        {
            context->SetArgByte(index, value);
        }

        void operator()(std::uint16_t value) const
        {
            context->SetArgWord(index, value);
        }

        void operator()(std::uint32_t value) const
        {
            context->SetArgDWord(index, value);
        }

        void operator()(std::uint64_t value) const
        {
            context->SetArgQWord(index, value);
        }

        void operator()(float value) const
        {
            context->SetArgFloat(index, value);
        }

        void operator()(double value) const
        {
            context->SetArgDouble(index, value);
        }
    };

    void PushVariantToParameter(ParameterIndex index, Variant& variant, asIScriptContext* context)
    {
        std::visit(VariantVisitImplementation{index, context}, variant);
    }
}