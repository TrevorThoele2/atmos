#include "AngelScriptFunctionManagement.h"

#include <angelscript.h>
#include "AngelScriptPrimitive.h"
#include "AngelScriptHandle.h"

#include <Chroma/Iterate.h>

namespace Atmos::Scripting::Angel
{
    asITypeInfo& RequiredTypeInfoByDeclaration(const String& declaration, asIScriptGeneric& generic)
    {
        const auto found = generic.GetEngine()->GetTypeInfoByDecl(declaration.c_str());
        if (found)
            return *found;

        throw AngelScriptFailed(
            "Could not find object type.",
            {
                { "Declaration", declaration }
            });
    }

    void Dispatch<bool>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgByte(index, object));
    }

    void Dispatch<bool>::PushToReturn(bool object, asIScriptGeneric& generic)
    {
        generic.SetReturnByte(object);
    }

    auto Dispatch<bool>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> bool
    {
        return generic.GetArgByte(index);
    }

    auto Dispatch<bool>::PullFromReturn(asIScriptContext& context) -> bool
    {
        return context.GetReturnByte();
    }

    void Dispatch<std::int8_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgByte(index, object));
    }

    void Dispatch<std::int8_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnByte(object);
    }

    auto Dispatch<std::int8_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgByte(index);
    }

    auto Dispatch<std::int8_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnByte();
    }

    void Dispatch<std::int16_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgWord(index, object));
    }

    void Dispatch<std::int16_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnWord(object);
    }

    auto Dispatch<std::int16_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgWord(index);
    }

    auto Dispatch<std::int16_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnWord();
    }

    void Dispatch<std::int32_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgDWord(index, object));
    }

    void Dispatch<std::int32_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnDWord(object);
    }

    auto Dispatch<std::int32_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgDWord(index);
    }

    auto Dispatch<std::int32_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnDWord();
    }

    void Dispatch<std::int64_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgQWord(index, object));
    }

    void Dispatch<std::int64_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnQWord(object);
    }

    auto Dispatch<std::int64_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgQWord(index);
    }

    auto Dispatch<std::int64_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnQWord();
    }

    void Dispatch<std::uint8_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgByte(index, object));
    }

    void Dispatch<std::uint8_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnByte(object);
    }

    auto Dispatch<std::uint8_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgByte(index);
    }

    auto Dispatch<std::uint8_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnByte();
    }

    void Dispatch<std::uint16_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgWord(index, object));
    }

    void Dispatch<std::uint16_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnWord(object);
    }

    auto Dispatch<std::uint16_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgWord(index);
    }

    auto Dispatch<std::uint16_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnWord();
    }

    void Dispatch<std::uint32_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgDWord(index, object));
    }

    void Dispatch<std::uint32_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnDWord(object);
    }

    auto Dispatch<std::uint32_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgDWord(index);
    }

    auto Dispatch<std::uint32_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnDWord();
    }

    void Dispatch<std::uint64_t>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgQWord(index, object));
    }

    void Dispatch<std::uint64_t>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnQWord(object);
    }

    auto Dispatch<std::uint64_t>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgQWord(index);
    }

    auto Dispatch<std::uint64_t>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnQWord();
    }

    void Dispatch<float>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgFloat(index, object));
    }

    void Dispatch<float>::PushToReturn(Type object, asIScriptGeneric& generic)
    {
        generic.SetReturnFloat(object);
    }

    auto Dispatch<float>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> float
    {
        return generic.GetArgFloat(index);
    }

    auto Dispatch<float>::PullFromReturn(asIScriptContext& context) -> float
    {
        return context.GetReturnFloat();
    }

    void Dispatch<double>::PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
    {
        VerifyResult(context.SetArgDouble(index, object));
    }

    void Dispatch<double>::PushToReturn(double object, asIScriptGeneric& generic)
    {
        generic.SetReturnDouble(object);
    }

    auto Dispatch<double>::PullFromParameter(ParameterIndex index, asIScriptGeneric& generic) -> Type
    {
        return generic.GetArgDouble(index);
    }

    auto Dispatch<double>::PullFromReturn(asIScriptContext& context) -> Type
    {
        return context.GetReturnDouble();
    }

    struct DispatchPushToParameterVisitImplementation
    {
        ParameterIndex index;
        asIScriptContext* context;

        DispatchPushToParameterVisitImplementation(ParameterIndex index, asIScriptContext* context) : index(index), context(context)
        {}

        template<class T>
        void operator()(T object) const
        {
            Dispatch<T>::PushToParameter(index, object, *context);
        }
    };

    void PushVariantToParameter(ParameterIndex index, Variant object, asIScriptContext& context)
    {
        std::visit(DispatchPushToParameterVisitImplementation{ index, &context }, object);
    }

    template<size_t i>
    struct DispatchPullFromReturnIDVisitImplementation
    {
        static bool Check(Variant& out, int typeID, asIScriptContext& context)
        {
            using Type = std::variant_alternative_t<i, Variant>;

            auto checkTypeID = Registration<Type>::typeID;
            if (checkTypeID && typeID == *checkTypeID)
            {
                out = Dispatch<Type>::PullFromReturn(context);
                return true;
            }
            else
                return false;
        }
    };

    template<size_t i>
    struct DispatchPullFromReturnNameVisitImplementation
    {
        static bool Check(Variant& out, String returnTypeName, asIScriptContext& context)
        {
            using Type = std::variant_alternative_t<i, Variant>;

            if (returnTypeName == Registration<Type>::Name())
            {
                out = Dispatch<Type>::PullFromReturn(context);
                return true;
            }
            else
                return false;
        }
    };

    auto PullVariantFromReturn(asIScriptContext& context) -> Variant
    {
        Variant out;

        const auto returnTypeID = context.GetFunction()->GetReturnTypeId();
        const auto typeInfo = context.GetEngine()->GetTypeInfoById(returnTypeID);
        if (typeInfo)
        {
            const auto returnTypeName = typeInfo->GetName();

            Chroma::IterateRangeCheckStop<size_t, DispatchPullFromReturnNameVisitImplementation, bool, std::variant_size_v<Variant> -1>(
                true, out, returnTypeName, context);
        }
        else
        {
            Chroma::IterateRangeCheckStop<size_t, DispatchPullFromReturnIDVisitImplementation, bool, std::variant_size_v<Variant> -1>(
                true, out, returnTypeID, context);
        }

        return out;
    }
}