#pragma once

#include <angelscript.h>
#include "AngelScriptRegistration.h"
#include "String.h"
#include <vector>

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<void>
    {
        static String Name() { return "void"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_VOID;
    };

    template<>
    struct Registration<bool>
    {
        static String Name() { return "bool"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_BOOL;
    };

    template<>
    struct Registration<std::int8_t>
    {
        static String Name() { return "int8"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT8;
    };

    template<>
    struct Registration<std::int16_t>
    {
        static String Name() { return "int16"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT16;
    };

    template<>
    struct Registration<std::int32_t>
    {
        static String Name() { return "int"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT32;
    };

    template<>
    struct Registration<std::int64_t>
    {
        static String Name() { return "int64"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT64;
    };

    template<>
    struct Registration<std::uint8_t>
    {
        static String Name() { return "uint8"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT8;
    };

    template<>
    struct Registration<std::uint16_t>
    {
        static String Name() { return "uint16"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT16;
    };

    template<>
    struct Registration<std::uint32_t>
    {
        static String Name() { return "uint"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT32;
    };

    template<>
    struct Registration<std::uint64_t>
    {
        static String Name() { return "uint64"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT64;
    };

    template<>
    struct Registration<float>
    {
        static String Name() { return "float"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_FLOAT;
    };

    template<>
    struct Registration<double>
    {
        static String Name() { return "double"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_DOUBLE;
    };

    template<>
    struct Registration<std::string>
    {
        static String Name() { return "string"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = {};
    };

    template<class T>
    struct Registration<std::vector<T>>
    {
        static String Name() { return CreateName({ Registration<T>::ContainingNamespace() }, Registration<T>::Name()) + "[]"; }
        static String ContainingNamespace() { return ""; }
        static String Documentation() { return ""; }
        static const ObjectType objectType = ObjectType::Handle;
        static inline std::optional<int> typeID = {};
    };
}