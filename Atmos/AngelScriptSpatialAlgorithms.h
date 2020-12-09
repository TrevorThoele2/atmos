#pragma once

#include "AngelScriptGlobalManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    class SpatialAlgorithms
    {};

    template<>
    struct Registration<SpatialAlgorithms>
    {
        using Management = GlobalManagement;

        static inline const String name = "SpatialAlgorithms";
        static inline const String containingNamespace = Namespaces::Atmos::Spatial::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    };
}