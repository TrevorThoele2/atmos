#pragma once

#include <angelscript.h>
#include "AngelScriptRegistration.h"
#include "AngelScriptNamespaces.h"
#include "String.h"

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<void>
    {
        static inline const String name = "void";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_VOID;
    };

    template<>
    struct Registration<bool>
    {
        static inline const String name = "bool";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_BOOL;
    };

    template<>
    struct Registration<std::int8_t>
    {
        static inline const String name = "int8";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT8;
    };

    template<>
    struct Registration<std::int16_t>
    {
        static inline const String name = "int16";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT16;
    };

    template<>
    struct Registration<std::int32_t>
    {
        static inline const String name = "int";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT32;
    };

    template<>
    struct Registration<std::int64_t>
    {
        static inline const String name = "int64";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_INT64;
    };

    template<>
    struct Registration<std::uint8_t>
    {
        static inline const String name = "uint8";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT8;
    };

    template<>
    struct Registration<std::uint16_t>
    {
        static inline const String name = "uint16";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT16;
    };

    template<>
    struct Registration<std::uint32_t>
    {
        static inline const String name = "uint";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT32;
    };

    template<>
    struct Registration<std::uint64_t>
    {
        static inline const String name = "uint64";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_UINT64;
    };

    template<>
    struct Registration<float>
    {
        static inline const String name = "float";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_FLOAT;
    };

    template<>
    struct Registration<double>
    {
        static inline const String name = "double";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = asTYPEID_DOUBLE;
    };

    template<>
    struct Registration<std::string>
    {
        static inline const String name = "string";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Value;
        static inline std::optional<int> typeID = {};
    };

    template<class T>
    struct Registration<std::vector<T>>
    {
        static inline const String name = CreateName({ Registration<T>::containingNamespace }, Registration<T>::name) + "[]";
        static inline const String containingNamespace = Namespaces::name;
        static const ObjectType objectType = ObjectType::Handle;
        static inline std::optional<int> typeID = {};
    };
}