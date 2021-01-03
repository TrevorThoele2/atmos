#pragma once

#include "Entity.h"

#include "AngelScriptRegistration.h"
#include "AngelScriptGlobalManagement.h"

#include "String.h"

class asIScriptEngine;

namespace Atmos::Scripting::Angel
{
    class CurrentEntity
    {};

    template<>
    struct Registration<CurrentEntity>
    {
        static inline const String name = "CurrentEntity";
        static inline const String containingNamespace = "Atmos";
        static inline const String documentation = "Represents the currently executing entity.";
        static const ObjectType objectType = ObjectType::Value;

        using Management = GlobalManagement;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Entity::Entity> Retrieve(Arca::Reliquary* reliquary);
    };
}