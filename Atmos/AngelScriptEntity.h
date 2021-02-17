#pragma once

#include "Entity.h"
#include "FindEntityByName.h"
#include "FindEntityByPosition.h"
#include "MoveEntityTo.h"
#include "CanMoveEntityTo.h"
#include "ModifyEntityTags.h"
#include "FindPath.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Entity::Entity>
    {
        using Type = Arca::Index<Entity::Entity>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Entity"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String DoName(Type type);
        [[nodiscard]] static Spatial::Grid::Point Position(Type type);
        [[nodiscard]] static bool IsSolid(Type type);
        [[nodiscard]] static std::vector<String> Tags(Type type);
    };

    template<>
    struct Registration<Arca::Index<Entity::Entity>> : Registration<Entity::Entity>
    {};

    template<>
    struct Registration<Entity::FindByName>
    {
        using Type = Entity::FindByName;
        using Management = ObjectManagement<Type>;

        static String Name() { return "FindByName"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Arca::Index<Entity::Entity> ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<>
    struct Registration<Entity::FindByPosition>
    {
        using Type = Entity::FindByPosition;
        using Management = ObjectManagement<Type>;

        static String Name() { return "FindByPosition"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static std::vector<Arca::Index<Entity::Entity>> ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<>
    struct Registration<Entity::MoveTo>
    {
        using Type = Entity::MoveTo;
        using Management = ObjectManagement<Type>;

        static String Name() { return "MoveTo"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Entity::CanMoveTo>
    {
        using Type = Entity::CanMoveTo;
        using Management = ObjectManagement<Type>;

        static String Name() { return "CanMoveTo"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static bool ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };

    template<>
    struct Registration<Entity::ModifyTags>
    {
        using Type = Entity::ModifyTags;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ModifyTags"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    };

    template<>
    struct Registration<Entity::FindPath>
    {
        using Type = Entity::FindPath;
        using Management = ObjectManagement<Type>;

        static String Name() { return "FindPath"; }
        static String ContainingNamespace() { return "Atmos::Entity"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static std::vector<Spatial::Grid::Point> ToCommandReturn(Arca::command_result_t<Type> fromArca, Arca::Reliquary&);
    };
}