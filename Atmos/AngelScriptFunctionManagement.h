#pragma once

#include "ScriptVariant.h"

#include <angelscript.h>
#include "AngelScriptResultVerification.h"
#include "AngelScriptRegistration.h"
#include <scriptarray.h>

namespace Atmos::Scripting::Angel
{
    using ParameterIndex = unsigned int;

    asITypeInfo& RequiredTypeInfoByDeclaration(const String& declaration, asIScriptGeneric& generic);

    template<class T, class Enable = void>
    struct Dispatch
    {
        using Type = T;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
        {
            context.SetArgObject(index, &object);
        }

        static void PushToReturn(Type object, asIScriptGeneric& generic)
        {
            VerifyResult(generic.SetReturnObject(&object));
        }

        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic)
        {
            return *reinterpret_cast<Type*>(generic.GetArgObject(index));
        }

        static Type PullFromReturn(asIScriptContext& context)
        {
            return *reinterpret_cast<T*>(context.GetReturnObject());
        }
    };

    template<class T>
    struct Dispatch<T, std::enable_if_t<std::is_enum_v<T>, void>>
    {
        using Type = T;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
        {
            Dispatch<std::underlying_type_t<T>>::PushToParameter(index, static_cast<std::underlying_type_t<T>>(object), context);
        }

        static void PushToReturn(Type object, asIScriptGeneric& generic)
        {
            Dispatch<std::underlying_type_t<T>>::PushToReturn(static_cast<std::underlying_type_t<T>>(object), generic);
        }

        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic)
        {
            return static_cast<T>(Dispatch<std::underlying_type_t<T>>::PullFromParameter(index, generic));
        }

        static Type PullFromReturn(asIScriptContext& context)
        {
            return static_cast<T>(Dispatch<std::underlying_type_t<T>>::PullFromReturn(context));
        }
    };

    template<class T>
    struct Dispatch<T*>
    {
        using Type = T*;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
        {
            context.SetArgObject(index, object);
        }

        static void PushToReturn(Type object, asIScriptGeneric& generic)
        {
            VerifyResult(generic.SetReturnObject(object));
        }

        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic)
        {
            return reinterpret_cast<Type>(generic.GetArgAddress(index));
        }

        static Type PullFromReturn(asIScriptContext& context)
        {
            return reinterpret_cast<T*>(context.GetReturnObject());
        }
    };

    template<>
    struct Dispatch<bool>
    {
        using Type = bool;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::int8_t>
    {
        using Type = std::int8_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::int16_t>
    {
        using Type = std::int16_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::int32_t>
    {
        using Type = std::int32_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::int64_t>
    {
        using Type = std::int64_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::uint8_t>
    {
        using Type = std::uint8_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::uint16_t>
    {
        using Type = std::uint16_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::uint32_t>
    {
        using Type = std::uint32_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<std::uint64_t>
    {
        using Type = std::uint64_t;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<float>
    {
        using Type = float;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<>
    struct Dispatch<double>
    {
        using Type = double;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context);
        static void PushToReturn(Type object, asIScriptGeneric& generic);
        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic);
        static Type PullFromReturn(asIScriptContext& context);
    };

    template<class T>
    struct Dispatch<std::vector<T>>
    {
        using Type = std::vector<T>;

        static void PushToParameter(ParameterIndex index, Type object, asIScriptContext& context)
        {
            context.SetArgObject(index, object);
        }

        static void PushToReturn(Type object, asIScriptGeneric& generic)
        {
            auto& typeInfo = RequiredTypeInfoByDeclaration(
                "array<" + CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + ">",
                generic);
            auto scriptObject = CScriptArray::Create(&typeInfo, object.size());

            for (size_t i = 0; i < object.size(); ++i)
                scriptObject->SetValue(i, &object[i]);

            Dispatch<CScriptArray*>::PushToReturn(scriptObject, generic);

            scriptObject->Release();
        }

        static Type PullFromParameter(ParameterIndex index, asIScriptGeneric& generic)
        {
            auto pulled = Dispatch<CScriptArray*>::PullFromParameter(index, generic);

            std::vector<T> returnValue;
            returnValue.reserve(pulled->GetSize());
            for (asUINT i = 0; i < pulled->GetSize(); ++i)
                returnValue.push_back(*reinterpret_cast<T*>(pulled->At(i)));

            pulled->Release();

            return returnValue;
        }

        static Type PullFromReturn(asIScriptContext& context)
        {
            return reinterpret_cast<T*>(context.GetReturnObject());
        }
    };

    template<class T>
    void PushToParameter(ParameterIndex index, T object, asIScriptContext& context)
    {
        Dispatch<T>::PushToParameter(index, object, context);
    }

    template<ParameterIndex index, class T>
    void PushToParameter(T object, asIScriptContext& context)
    {
        Dispatch<T>::PushToParameter(index, object, context);
    }

    template<class T>
    void PushToReturn(T object, asIScriptGeneric& generic)
    {
        Dispatch<T>::PushToReturn(object, generic);
    }

    template<class T>
    T PullFromParameter(ParameterIndex index, asIScriptGeneric& generic)
    {
        return Dispatch<T>::PullFromParameter(index, generic);
    }

    template<ParameterIndex index, class T>
    T PullFromParameter(asIScriptGeneric& generic)
    {
        return Dispatch<T>::PullFromParameter(index, generic);
    }

    template<class T>
    T PullFromReturn(asIScriptContext& context)
    {
        return Dispatch<T>::PullFromReturn(context);
    }

    void PushVariantToParameter(ParameterIndex index, Variant object, asIScriptContext& context);
    Variant PullVariantFromReturn(asIScriptContext& context);
}